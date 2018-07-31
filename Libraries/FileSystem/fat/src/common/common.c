/****************************************************************************
 *
 *            Copyright (c) 2003-2008 by HCC Embedded
 *
 * This software is copyrighted by and is the sole property of
 * HCC.  All rights, title, ownership, or other interests
 * in the software remain the property of HCC.  This
 * software may only be used in accordance with the corresponding
 * license agreement.  Any unauthorized use, duplication, transmission,
 * distribution, or disclosure of this software is expressly forbidden.
 *
 * This Copyright notice may not be removed or modified without prior
 * written consent of HCC.
 *
 * HCC reserves the right to modify this software without notice.
 *
 * HCC Embedded
 * Budapest 1133
 * Vaci ut 110
 * Hungary
 *
 * Tel:  +36 (1) 450 1302
 * Fax:  +36 (1) 450 1303
 * http: www.hcc-embedded.com
 * email: info@hcc-embedded.com
 *
 ***************************************************************************/

#include "fat.h"
#include "port_f.h"
#include "common.h"
#include "debug.h"

/****************************************************************************
 *
 * variables and definitions
 *
 ***************************************************************************/

FN_FILESYSTEM f_filesystem;
FN_MUTEX_TYPE fat_gmutex;

#if F_FILE_CHANGED_EVENT && (!FN_CAPI_USED)
F_FILE_CHANGED_EVENTFUNC f_filechangedevent;
#endif

#if (!FN_CAPI_USED)
#ifdef USE_MALLOC
F_MULTI *g_multi[FN_MAXTASK];		/* for multi-task access */
#else
F_MULTI g_multi[FN_MAXTASK];		/* for multi-task access */
#endif
#endif

/* structure for sector and cluster definitions for FAT32 */
typedef struct
{
	unsigned long max_sectors;
	unsigned char sector_per_cluster;
} t_FAT32_CS;

/* Defines the number of sectors per cluster on a sector number basis */
static const t_FAT32_CS FAT32_CS[]={
	{ 0x00020000, 1 },	/* ->64MB */
	{ 0x00040000, 2 },	/* ->128MB */
	{ 0x00080000, 4 },	/* ->256MB */
	{ 0x01000000, 8 },	/* ->8GB */
	{ 0x02000000, 16 },	/* ->16GB */
	{ 0x0ffffff0, 32 }	/* -> ... */
};

/****************************************************************************
 *
 * fn_getversion
 *
 * returns with the filesystem version string
 *
 * RETURNS
 *
 * string pointer with vfersion number
 *
 ***************************************************************************/

char *fn_getversion(void) {
#ifndef HCC_UNICODE
#if (!F_LONGFILENAME)
	return (char*)("HCC_FAT ver:3.72");
#else
	return (char*)("HCC_FAT_LFN ver:3.72");
#endif
#else
	return (char*)("HCC_FAT_LFN_UNI ver:3.72");
#endif
}

/****************************************************************************
 *
 * fn_init
 *
 * initialize fat file system, this function must be called at startup
 *
 * RETURNS
 *
 * error code or zero if successful
 *
 ***************************************************************************/

int fn_init(void)
{
	DEBOPEN;

#if F_FILE_CHANGED_EVENT
	f_filechangedevent=0;
#endif

	(void)_memset(&f_filesystem,0,sizeof(f_filesystem)); /* reset filesystem */

#if (!FN_CAPI_USED)
#ifndef USE_MALLOC
	(void)_memset(g_multi,0,sizeof(g_multi));
#endif
#endif

#if (!USE_TASK_SEPARATED_CWD)
#if (!FN_CAPI_USED)
	f_filesystem.f_curdrive=FN_CURRDRIVE; 
#else
	f_filesystem.f_curdrive=0; 
#endif
#endif

	if (f_mutex_create(&fat_gmutex)) return F_ERR_INITFUNC;

	return F_NO_ERROR;
}

/****************************************************************************
 *
 * _f_get16bitl
 *
 * get a word 16bit number from a memory (FAT uses LITTLE ENDIAN mode always)
 *
 * INPUTS
 *
 * ptr - pointer where data is
 *
 * RETURNS
 *
 * word number
 *
 ***************************************************************************/

unsigned short _f_get16bitl(void *ptr)
{
	unsigned char *cptr=(unsigned char*)ptr;
	unsigned short ret;

	ret=(unsigned short)(cptr[1] & 0x00ff);	/* 0x00ff needed because of DSPs */
	ret<<=8;
	ret|=(unsigned short)(cptr[0] & 0x00ff);

	return ret;
}

/****************************************************************************
 *
 * _f_set16bitl
 *
 * set a word 16bit number into a memory (FAT uses LITTLE ENDIAN mode always)
 *
 * INPUTS
 *
 * ptr - where to store data
 * num - 16 bit number to store
 *
 ***************************************************************************/

void _f_set16bitl(void *ptr,unsigned short num)
{
	unsigned char *cptr=(unsigned char*)ptr;

	cptr[0]=(unsigned char)( num     & 0x00ff);	  /* 0x00ff needed because of DSPs */
	cptr[1]=(unsigned char)((num>>8) & 0x00ff);
}

/****************************************************************************
 *
 * _f_get32bitl
 *
 * get a long 32bit number from a memory (FAT uses LITTLE ENDIAN mode always)
 *
 * INPUTS
 *
 * ptr - pointer where data is
 *
 * RETURNS
 *
 * long number
 *
 ***************************************************************************/

unsigned long _f_get32bitl(void *ptr)
{
	unsigned char *cptr=(unsigned char*)ptr;
	unsigned long ret;

	ret=(unsigned long)cptr[3] & 0x00ff;	/* 0x00ff needed because of DSPs */
	ret<<=8;
	ret|=(unsigned long)cptr[2] & 0x00ff;
	ret<<=8;
	ret|=(unsigned long)cptr[1] & 0x00ff;
	ret<<=8;
	ret|=(unsigned long)cptr[0] & 0x00ff;

	return ret;
}

/****************************************************************************
 *
 * _f_set32bitl
 *
 * set a long 32bit number into a memory (FAT uses LITTLE ENDIAN mode always)
 *
 * INPUTS
 *
 * ptr - where to store data
 * num - 32 bit number to store
 *
 ***************************************************************************/

void _f_set32bitl(void *ptr,unsigned long num)
{
	unsigned char *cptr=(unsigned char*)ptr;

	cptr[0]=(unsigned char)( num      & 0x00ff); /* 0x00ff needed because of DSPs */
	cptr[1]=(unsigned char)((num>>8)  & 0x00ff);
	cptr[2]=(unsigned char)((num>>16) & 0x00ff);
	cptr[3]=(unsigned char)((num>>24) & 0x00ff);

}

/****************************************************************************
 *
 * _get8bit
 *
 * get characters from memory
 *
 * INPUTS
 *
 * array - where to store read data
 * num - number of characters
 * ptr - where to read from
 *
 * RETURNS
 *
 * read pointer new position
 *
 ***************************************************************************/

static unsigned char *_get8bit(unsigned char *array,long num,unsigned char *ptr)
{
   if (!array) return ptr+num;

   while (num--)
   {
      *array++=*ptr++;
   }

   return ptr;
}

/****************************************************************************
 *
 * _get16bitl
 *
 * get a 16bit number from memory
 *
 * INPUTS
 *
 * num - pointer where to store 16 bit read number
 * ptr - where to get the value from
 *
 * RETURNS
 *
 * read pointer new position
 *
 ***************************************************************************/

static unsigned char *_get16bitl(unsigned short *num,unsigned char *ptr)
{
	*num=_f_get16bitl(ptr);

	/* because on Intel sizeof(unsigned short)==2 (DSP checked) */
	return ptr+2; 
}

/****************************************************************************
 *
 * _get32bitl
 *
 * get a 32bit number from memory with little endian
 *
 * INPUTS
 *
 * num - pointer where to store 32 bit read number
 * ptr - where to get the value from
 *
 * RETURNS
 *
 * read pointer new position
 *
 ***************************************************************************/

static unsigned char *_get32bitl(unsigned long *num,unsigned char *ptr)
{
   *num=_f_get32bitl(ptr);

	/* because on Intel sizeof(unsigned long)==4 (DSP checked) */
   return ptr+4;
}

/****************************************************************************
 *
 * _set8bitzero
 *
 * fills with zero charater to memory
 *
 * INPUTS
 *
 * num - number of characters
 * ptr - where to store data
 *
 * RETURNS
 *
 * last write position
 *
 ***************************************************************************/

static unsigned char *_set8bitzero(long num,unsigned char *ptr)
{
	while (num--)
	{
		*ptr++=0;
	}

	return ptr;
}

/****************************************************************************
 *
 * _set8bit
 *
 * copy a charater string to memory
 *
 * INPUTS
 *
 * array - original code what to copy
 * num - number of characters
 * ptr - where to store data
 *
 * RETURNS
 *
 * last write position
 *
 ***************************************************************************/

static unsigned char *_set8bit(const unsigned char *array,long num,unsigned char *ptr)
{
	if (!array) return _set8bitzero(num,ptr);

	while (num--)
	{
		*ptr++=*array++;
	}

	return ptr;
}

/****************************************************************************
 *
 * _set16bitl
 *
 * store a 16bit word into memory with little endian
 *
 * INPUTS
 *
 * num - 16bit number to store
 * ptr - where to store data
 *
 * RETURNS
 *
 * last write position
 *
 ***************************************************************************/

static unsigned char *_set16bitl(unsigned short num,unsigned char *ptr)
{
	_f_set16bitl(ptr,num);

	/* because on Intel sizeof(unsigned short)==2 (DSP checked) */
	return ptr+2;
}

/****************************************************************************
 *
 * _set32bitl
 *
 * store a 32bit long number into memory
 *
 * INPUTS
 *
 * num - 32bit number to store
 * ptr - where to store data
 *
 * RETURNS
 *
 * last write position
 *
 ***************************************************************************/

static unsigned char *_set32bitl(unsigned long num,unsigned char *ptr)
{
	_f_set32bitl(ptr,num);

	/* because on Intel sizeof(unsigned long)==4 (DSP checked) */
	return ptr+4;
}

/****************************************************************************
 *
 * _f_setcardremoved
 *
 * sets all volume which is used a driver into F_STATE_NEED_MOUNT state
 *
 * INPUTS
 *	
 * driver - driver which is used by driver
 *
 ***************************************************************************/

static void _f_setcardremoved(F_DRIVER *driver)
{
	int vcou;
	/* set mount state to all partition */
	for (vcou=0; vcou<FN_MAXVOLUME; vcou++)
	{
		F_VOLUME *vi=&f_filesystem.volumes[vcou];

		if (vi->state)
		{
			if (vi->driver == driver)
			{
				vi->state=F_STATE_NEEDMOUNT; /* card has been removed; */
			}
		}
	}
}

/****************************************************************************
 *
 * _f_checkstatus
 *
 * checking a volume driver status, if media is removed or has been changed
 *
 * INPUTS
 *
 * vi - volume pointer
 * pstatus - pointer to a status variable to retreive current status
 *
 * RETURNS
 *
 * error code or zero if successful
 *
 ***************************************************************************/

int _f_checkstatus(F_VOLUME *vi,long *pstatus)
{
	/* reset status */
	if (pstatus) *pstatus=0;

	/* check if driver is existed */
	if (vi->driver)
	{
		/* check wether getstatus function is exist */
		if (vi->driver->getstatus)
		{
			long status=vi->driver->getstatus(vi->driver);

			/* store it */
			if (pstatus) *pstatus=status;

			/* check write protect flasg */
			if (status & F_ST_WRPROTECT)
			{
				if (vi->state == F_STATE_WORKING)
				{
					/* set write protect mode */
					vi->state = F_STATE_WORKING_WP; 
				}
			}

			/* checking if missing or removed (changed) */
			if (status & (F_ST_MISSING|F_ST_CHANGED))
			{
				_f_setcardremoved(vi->driver);
				return F_ERR_CARDREMOVED;
			}
		}
	}

	return F_NO_ERROR;
}

/****************************************************************************
 *
 * _f_ioctl
 *
 * pass i/o controll function to driver
 *
 * INPUTS
 *
 * vi - volume info 
 * msg - message to send
 * iparam - input structure parameter for the message 
 * oparam - output structure parameter for the message 
 *
 * RETURNS
 *
 * error code or zero if successful
 *
 ***************************************************************************/

static int _f_ioctl(F_VOLUME *vi, unsigned long msg, void *iparam, void *oparam)
{
	F_DRIVER *driver=vi->driver;
	if (driver)
	{
		if (driver->ioctl)
		{
			if (!driver->ioctl(driver,msg,iparam,oparam))
			{
				return F_NO_ERROR;
			}
		}
	}

	return F_ERR_NOTAVAILABLE;
}

/****************************************************************************
 *
 * _f_ioctl_endofdelete
 *
 * it calls driver's io ctrl function to signal end of delete function
 *
 * INPUTS
 *
 * vi - volume info 
 *
 * RETURNS
 *
 * error code or zero if successful
 *
 ***************************************************************************/

int _f_ioctl_endofdelete(F_VOLUME *vi)
{
	return _f_ioctl(vi,F_IOCTL_MSG_ENDOFDELETE,0,0);
}

/****************************************************************************
 *
 * _f_ioctl_multiplesectorerase
 *
 * execute an ioctl function which erases multiple sectors with buffer content
 *
 * INPUTS
 *
 * vi - volume info 
 * sector - start sector
 * sectorcou - number of sectors
 * buffer - one sector buffer data for erasing
 *
 * RETURNS
 *
 * error code or zero if successful
 *
 ***************************************************************************/

int _f_ioctl_multiplesectorerase(F_VOLUME *vi,unsigned long sector, unsigned long sectorcou, void *buffer)
{
	ST_IOCTL_MULTIPLESECTORERASE param;

	param.start_sector=sector;
	param.sector_num=sectorcou;
	param.one_sector_databuffer=buffer;

	return _f_ioctl(vi,F_IOCTL_MSG_MULTIPLESECTORERASE,&param,0);
}

/****************************************************************************
 *
 * _f_drvgetphy
 *
 * get physical information about a drive
 *
 * INPUTS
 *
 * driver - driver to get whom
 * phy - where to store information
 *
 * RETURNS
 *
 * error code or zero if successful
 *
 ***************************************************************************/

static int _f_drvgetphy(F_DRIVER *driver, F_PHY *phy)
{
	int ret;

	/* check driver 1st */
	if (!driver) return F_ERR_INITFUNC;

	/* check if we have physical information function about media */
	if (!driver->getphy) return F_ERR_INITFUNC;

	/* reset phy structure */
	(void)_memset(phy,0,sizeof(F_PHY));

	/* call driver getphy function */
	ret=driver->getphy(driver,phy);
	if (ret) return F_ERR_INVALIDMEDIA;

	/* check media descriptor it also supports multiple partition on removable media */
	if (!phy->media_descriptor)
	{
		phy->media_descriptor=F_MEDIADESC_REMOVABLE;
	}
	if (!phy->bytes_per_sector)
	{
		phy->bytes_per_sector=F_DEF_SECTOR_SIZE;
	}

	/* simple return with no error */
	return F_NO_ERROR;
}

/****************************************************************************
 *
 * _f_drvwritesector
 *
 * write a sector to a drive
 *
 * INPUTS
 *
 * driver - driver to get whom
 * data - where data is
 * sector - where to write it
 *
 * RETURNS
 *
 * error code or zero if successful
 *
 ***************************************************************************/

static int _f_drvwritesector(F_DRIVER *driver, void *data, unsigned long sector)
{
	int retry=RDWR_RETRY; /* set retry counter */
	int ret=0;

	if (!driver) return F_ERR_INITFUNC;
	if (!driver->writesector) return F_ERR_INITFUNC;

	while (retry--)
	{
		/* write sector */
		ret=driver->writesector(driver,data,sector);

		if (!ret) 
		{
			/* if success return with no error */
			return F_NO_ERROR;
		}

		/* -1 means media has been removed */
		if (ret==-1) 
		{
			_f_setcardremoved(driver);

			return F_ERR_CARDREMOVED;
		}
	}

	/* any error */
	return F_ERR_ONDRIVE;
}


/****************************************************************************
 *
 * _f_drvreadsector
 *
 * read a sector from drive
 *
 * INPUTS
 *
 * driver - driver to get whom
 * data - where data is
 * sector - where to read it
 *
 * RETURNS
 *
 * error code or zero if successful
 *
 ***************************************************************************/

int _f_drvreadsector(F_DRIVER *driver, void *data, unsigned long sector)
{
	int retry=RDWR_RETRY;

	while (retry--)
	{
		/* read sector */
		int ret=driver->readsector(driver,data,sector);

		if (!ret) 
		{
			return F_NO_ERROR;
		}

		if (ret==-1)
		{
			_f_setcardremoved(driver);
			return F_ERR_CARDREMOVED;
		}
	}

	return F_ERR_ONDRIVE;	/* error after retrying finished */
}

/****************************************************************************
 *
 * _f_writesector
 *
 * write sector data on a volume, it calls low level driver function, it
 * writes a complete sector
 *
 * INPUTS
 *
 * vi - volume pointer
 * data - data pointer where the data is
 * sector - which physical sector
 * cnt - number of sectors to write (max. 256)
 *
 * RETURNS
 *
 * error code or zero if successful
 *
 ***************************************************************************/

int _f_writesector(F_VOLUME *vi, void *data, unsigned long sector, int cnt)
{
	F_DRIVER *driver;

	/* check volumeinfo if it is exist */
	if (!vi) 
	{
		return F_ERR_INVALIDDRIVE;
	}

	/* get driver pointer */
	driver = vi->driver;

	/* check if driver exist */
	if (!driver)
	{
		return F_ERR_INVALIDDRIVE;
	}

	/* check if driver function is available */
	if (!driver->writesector)
	{
		return F_ERR_ACCESSDENIED;
	}

	if (cnt==1 || (!driver->writemultiplesector))
	{
		int i;

		/* check write protecting */
		if (vi->state == F_STATE_WORKING_WP) 
		{
			return F_ERR_WRITEPROTECT;
		}

		/* simple sector write */
		for (i=0;i<cnt;i++,sector++)
		{
			int ret;

			/* write data */
			ret=_f_drvwritesector(driver,(char *)data+i*(vi->bootrecord.bytes_per_sector),sector);

			if (ret) 
			{
				if (_f_checkstatus(vi,0)==F_ERR_CARDREMOVED)
				{
					return F_ERR_CARDREMOVED;
				}

				return ret;
			}
		}

		return F_NO_ERROR;
	}
	else
	{
		int retry=RDWR_RETRY;

		/* Multiple sector write */
		while (retry--)
		{
			int ret;

			/* check write protecting */
			if (vi->state == F_STATE_WORKING_WP) 
			{
				return F_ERR_WRITEPROTECT;
			}

			/* write multiple sector */
			ret=driver->writemultiplesector(driver,data,sector,cnt);
			if (!ret) 
			{
				return F_NO_ERROR;
			}

			/* check its status why it fails */
			if (_f_checkstatus(vi,0) == F_ERR_CARDREMOVED)
			{
				return F_ERR_CARDREMOVED;
			}

		    /* Check if card has been removed */
			if (ret==-1)
			{
				_f_setcardremoved(driver);
				return F_ERR_CARDREMOVED;
			}
		}
	}

	return F_ERR_ONDRIVE;
}

/****************************************************************************
 *
 * _f_readsector
 *
 * read sector data from a volume, it calls low level driver function, it
 * reads a complete sector
 *
 * INPUTS
 *
 * vi - volume pointer
 * data - data pointer where store data
 * sector - which physical sector is read
 * cnt - number of sectors to read (max. 256)
 *
 * RETURNS
 *
 * error code or zero if successful
 *
 ***************************************************************************/

int _f_readsector(F_VOLUME *vi,void *data, unsigned long sector,int cnt)
{
	F_DRIVER *driver;

	/* check volumeinfo if it is exist */
	if (!vi) 
	{
		return F_ERR_INVALIDDRIVE;
	}

	/* gets it driver */
	driver=vi->driver;

	/* check if driver exist */
	if (!driver)
	{
		return F_ERR_INVALIDDRIVE;
	}

	/* Check if read driver function is exist */
	if (!driver->readsector) 
	{
		return F_ERR_ACCESSDENIED;
	}

	if (cnt==1 || (!driver->readmultiplesector)) 
	{
		int i;

		/* simple sector read */
		for (i=0;i<cnt;i++,sector++)
		{
			int ret;

			ret=_f_drvreadsector(driver,(char *)data+i*(vi->bootrecord.bytes_per_sector),sector);

			if (ret)
			{
				if (_f_checkstatus(vi,0)==F_ERR_CARDREMOVED)
				{
					return F_ERR_CARDREMOVED;
				}

				return ret;
			}
		}

		return F_NO_ERROR;
	}
	else 
	{
		int retry=RDWR_RETRY;

		/* multiple sector read */
		while (retry--)
		{
			int ret;

			/* multiple sector read */
			ret=driver->readmultiplesector(driver,data,sector,cnt);
			if (!ret) 
			{
				return F_NO_ERROR;
			}

			/* check its status why it fails */
			if (_f_checkstatus(vi,0) == F_ERR_CARDREMOVED)
			{
				return F_ERR_CARDREMOVED;
			}

		    /* Check if card has been removed */
			if (ret==-1)
			{
				_f_setcardremoved(driver);
				return F_ERR_CARDREMOVED;
			}
		}
	}

	return F_ERR_ONDRIVE;
}


/****************************************************************************
 *
 * _f_memset
 *
 * standard memory set into a value
 *
 * INPUTS
 *
 * d - destination address
 * fill - what char used to fill
 * len - length of copied data
 *
 * RETURN
 *
 * original destination address
 *
 ***************************************************************************/

#ifdef INTERNAL_MEMFN

void *_f_memset(void *d, int fill, unsigned long len)
{
	char *dc=(char*)d;

    while(len--)
	{
      *dc++=(unsigned char)fill;
    }

    return d;
}

#endif

/****************************************************************************
 *
 * _f_memcpy
 *
 * standard memory copy
 *
 * INPUTS
 *
 * d - destination address
 * s - source address
 * len - length of copied data
 *
 * RETURN
 *
 * original destination address
 *
 ***************************************************************************/

#ifdef INTERNAL_MEMFN

void *_f_memcpy(void *d, void *s, unsigned long len)
{
	char *dc=(char*)d;
	char *sc=(char*)s;

    while(len--)
	{
      *dc++=*sc++;
    }

    return d;
}

#endif

/****************************************************************************
 *
 * _f_getmaxcluster
 *
 * calculate the maximum cluster number from boot record
 *
 * INPUTS
 *
 * vi - volume info
 *
 * RETURNS
 *
 * maximum number of clusters
 *
 ***************************************************************************/

unsigned long _f_getmaxcluster(const F_VOLUME *vi)
{
	unsigned long maxcluster=vi->bootrecord.number_of_sectors_less32; /* get the number of sector <32MB */

	/* if its 0 then get the number of sectors from >32MB */
	if (!maxcluster)
	{
		maxcluster=vi->bootrecord.number_of_sectors;
	}

	/* if we have sector number then convert data sectors to cluster number */
	if (maxcluster)
	{
		maxcluster+=vi->bootrecord.number_of_hidden_sectors; /* translation to last sector position */
		maxcluster-=vi->data.sector;						 /* substract the 1st data sector position */
		maxcluster/=vi->bootrecord.sector_per_cluster;		 /* divide by the sector per cluster */
	}

	return maxcluster;
}

/****************************************************************************
 *
 * _getsectorfromcluster
 *
 * retreives sector number from cluster
 * It doesn't check if cluster number is valid
 *
 * INPUTS
 *
 * vi - volume pointer
 * cluster - cluster number
 *
 * RETURNS
 *
 * sector number
 *
 ***************************************************************************/

static unsigned long _getsectorfromcluster(const F_VOLUME *vi,unsigned long cluster)
{
	unsigned long sector;

	sector=cluster;								/* get the value */
	sector-=2;									/* 1st 2 cluster is reserved */
 	sector*=vi->bootrecord.sector_per_cluster;	/* multiple with sector per cluster */
 	sector+=vi->data.sector;					/* translation to data sector start */

	return sector;								/* returns with the sector number */
}

/****************************************************************************
 *
 * _f_buildsectors
 *
 * calculate relative sector position from boot record
 *
 * INPUTS
 *
 * vi - volume pointer
 *
 ***************************************************************************/

static void _f_buildsectors(F_VOLUME *vi)
{
	F_BOOTRECORD *bootrecord=&vi->bootrecord;

	vi->mediatype=F_UNKNOWN_MEDIA;

	vi->firstfat.sector=bootrecord->reserved_sectors;
	vi->firstfat.sector+=bootrecord->number_of_hidden_sectors; /* maybe partioned */

	if (bootrecord->sector_per_FAT)
	{
		vi->firstfat.num=bootrecord->sector_per_FAT;

		/* calculating starting sector of root directory */
		vi->root.sector=vi->firstfat.sector;
		vi->root.sector+=vi->firstfat.num*bootrecord->number_of_FATs;

		/* calculating number of sectors of root directory */
		vi->root.num=bootrecord->max_root_entry;
		vi->root.num*=sizeof(F_DIRENTRY);
		vi->root.num/=bootrecord->bytes_per_sector;

		/* calculating starting sector of data area */
		vi->data.sector=vi->root.sector+vi->root.num;
		vi->data.num=0; /* number of data sector is not used */
	}
	else
	{ /* FAT 32 */
		vi->firstfat.num=bootrecord->sector_per_FAT32;

		/* calculating starting sector of data area */
		vi->data.sector=vi->firstfat.sector;
		vi->data.sector+=vi->firstfat.num*bootrecord->number_of_FATs;
		vi->data.num=0; /* number of data sector is not used */

		/* calculating starting sector of root directory */
		vi->root.sector=_getsectorfromcluster(vi,bootrecord->rootcluster);
		vi->root.num=bootrecord->sector_per_cluster;
	}

	/* support always smaller FAT */
	if (_f_getmaxcluster(vi)<F_FAT12_MAX_CLUSTER)
	{
		vi->mediatype=F_FAT12_MEDIA;
	}
	else if (_f_getmaxcluster(vi)<F_FAT16_MAX_CLUSTER)
	{
		vi->mediatype=F_FAT16_MEDIA;
	}
	else
	{
		vi->mediatype=F_FAT32_MEDIA;
	}

}

/****************************************************************************
 *
 * _f_getbootrecord
 *
 * searching and reading requested partition and get BootRecrord from it
 *
 * INPUTS
 *
 * vi - volume pointer
 * ptr - where to read it
 *
 * RETURNS
 *
 * error code or zero if successful
 *
 ***************************************************************************/

static int _f_getbootrecord(F_VOLUME *vi,unsigned char *ptr)
{
	unsigned long extpartsec=0;
	unsigned long extpartsecrel=0;
	int partition=vi->partition;
	int ret;

	/* set 1st sector position */
	vi->sectorstart=0;

	/* check if driver exist */
	if (!vi->driver)
	{
		return F_ERR_INVALIDDRIVE;
	}

	/* get its physical */
	ret=_f_drvgetphy(vi->driver,&vi->phy);
	if (ret) return ret;
	if (vi->phy.bytes_per_sector>F_MAX_SECTOR_SIZE)
	{
		return F_ERR_NOTSUPPSECTORSIZE;
	}

	/* set number of sector */
	vi->sectornum=vi->phy.number_of_sectors;
	if (!vi->sectornum) return F_ERR_INVALIDMEDIA;

	for(;;)
	{

		/* read BootRecord or MasterBootRecord */
		ret=_f_readsector(vi,ptr,vi->sectorstart,1);
		if (ret) return ret;

		/* 0x55aa must be at the end of sector */
		if ((ptr[0x1fe]!=0x55) || (ptr[0x1ff]!=0xaa))
		{
			if (!partition)
			{
				return F_ERR_NOTFORMATTED; /* if we found it */
			}

			return F_ERR_INVALIDMEDIA; /* can't find requested partition */
		}


		/* check if a boot record found */
		if ((ptr[0]==0xeb) || (ptr[0]==0xe9))
		{
			if (!partition)
			{
				return F_NO_ERROR; /* we find it */
			}

			/* boot record found but this is not the requested one */
			return F_ERR_INVALIDMEDIA; /* can't find requested partition */
		}
		else
		{	/* check for MasterBootRecord */
			int partentry;
			int found=0;

			for (partentry=0; partentry<=0x30; partentry+=0x10) /* 4 times 16 bytes entry */
			{
				unsigned char sysind=ptr[partentry+0x04+0x1be];
				unsigned long sectorstart;

				if (!sysind) continue;

				/* start,sizein sector for partition */
				sectorstart=_f_get32bitl(&ptr [partentry+0x08+0x1be] );
				vi->sectorstart=sectorstart+extpartsecrel+extpartsec; /* add relativ */
				vi->sectornum  =_f_get32bitl(&ptr [partentry+0x0C+0x1be] );

				found=1;

				/* check if extended */
				if (sysind==F_SYSIND_EXTWIN || sysind==F_SYSIND_EXTDOS)
				{
					vi->sectorstart=sectorstart+extpartsec; /* calc in this way */

					if (!extpartsec)
					{
						extpartsec=sectorstart; /* 1st extended partition */
						extpartsecrel=0;	/* current relative position */
					}
					else
					{
						extpartsecrel=sectorstart; /* chained extended partition */
					}

					break; /* if extended partition is found stop searching next entry */
				}


				if (partition)
				{
					partition--; /* decrease partition counter */
				}
				else
				{
					break;
				}
			}
			if (!found) return F_ERR_INVALIDMEDIA; /* can't find requested partition */
		}
	}
}


/****************************************************************************
 *
 * fn_getpartition
 *
 * getting partition info from drive
 *
 * INPUTS
 *
 * driver - driver structure
 * parnum - number of partition entry in par parameter
 * par - F_PARTITION structure where the result goes to
 *
 * RETURNS
 *
 * error code or zero if successful
 *
 ***************************************************************************/

int fn_getpartition(F_DRIVER *driver,int parnum, F_PARTITION *par)
{
	unsigned char sectorbuffer[F_MAX_SECTOR_SIZE];
	unsigned char *ptr=sectorbuffer;
	unsigned long extpartsec=0;
	unsigned long extpartsecrel=0;
	unsigned long sectorstart;
	unsigned long sectornum;
	F_PHY phy;
	int ret;

	if (parnum<=0) return F_ERR_MEDIATOOLARGE; /* not enought space in par structure */
	/* reset all entry to zero */
	(void)_memset(par,0,parnum*sizeof(F_PARTITION));

	/* set 1st sector position */
	sectorstart=0;

	/* check if driver exist */
	if (!driver) return F_ERR_INVALIDDRIVE;

	/* get its physical */
	ret=_f_drvgetphy(driver,&phy);
	if (ret) return ret;

	/* set number of sector */
	sectornum=phy.number_of_sectors;
	if (!sectornum) return F_ERR_INVALIDMEDIA;

	if (par)
	{
		par->secnum=sectornum;
	}
	else
	{
	 	return F_ERR_MEDIATOOLARGE; /* not enought space in par structure */
	}

	for(;;)
	{

		/* read BootRecord or MasterBootRecord */
		ret=_f_drvreadsector(driver,ptr,sectorstart);
		if (ret) return ret;

		/* 0x55aa must be at the end of sector */
		if ((ptr[0x1fe]!=0x55) || (ptr[0x1ff]!=0xaa))
		{
			return F_NO_ERROR; /* no more signature found */
		}

		/* check if a boot record found */
		if ((ptr[0]==0xeb) || (ptr[0]==0xe9))
		{
			return F_NO_ERROR; /* no more MBR found */
		}
		else
		{	/* check for MasterBootRecord */
			int partentry;
			int found=0;

			for (partentry=0; partentry<=0x30; partentry+=0x10) /* 4 times 16 bytes entry */
			{
				unsigned char sysind=ptr[partentry+0x04+0x1be];
				unsigned long secstart;

				if (!sysind) continue;

				/* start,sizein sector for partition */
				secstart=_f_get32bitl(&ptr [partentry+0x08+0x1be] );
				sectorstart=secstart+extpartsecrel+extpartsec; /* add relativ */
				sectornum  =_f_get32bitl(&ptr [partentry+0x0C+0x1be] );


				/* check if extended */
				if (sysind==F_SYSIND_EXTWIN || sysind==F_SYSIND_EXTDOS)
				{
					sectorstart=secstart+extpartsec; /* calc in this way */

					found=1;

					if (!extpartsec)
					{
						extpartsec=secstart; /* 1st extended partition */
						extpartsecrel=0;	/* current relative position */
					}
					else
					{
						extpartsecrel=secstart; /* chained extended partition */
					}

					break; /* if extended partition is found stop searching next entry */
				}


				if (sectornum)
				{
					if (par)
					{
						par->secnum=sectornum;
						par->system_indicator=sysind;
						par++;
						parnum--;
						if (!parnum) par=0;
					}
					else
					{
						return F_ERR_MEDIATOOLARGE; /* not enought space in par structure */
					}
				}
			}
			if (!found) return F_NO_ERROR; /* no more entry found */
		}
	}
}

/****************************************************************************
 *
 * _f_readbootrecord
 *
 * read boot record from a volume, it detects if there is MBR on the media
 *
 * INPUTS
 *
 * vi - volume pointer
 *
 * RETURNS
 *
 * error code or zero if successful
 *
 ***************************************************************************/

static int _f_readbootrecord(F_VOLUME *vi)
{
	unsigned char *ptr=vi->sectorbuffer;
	F_BOOTRECORD *f_bootrecord=&vi->bootrecord;
	int ret;

	(void)_memset(f_bootrecord,0,sizeof(F_BOOTRECORD));

	ret=_f_getbootrecord(vi,ptr);
	if (ret) return ret;

	ptr=_get8bit(f_bootrecord->jump_code,sizeof(f_bootrecord->jump_code),ptr);
	ptr=_get8bit(f_bootrecord->OEM_name,sizeof(f_bootrecord->OEM_name),ptr);
	ptr=_get16bitl(&f_bootrecord->bytes_per_sector,ptr);

	if (f_bootrecord->bytes_per_sector>F_MAX_SECTOR_SIZE)
	{
		vi->mediatype=F_UNKNOWN_MEDIA;
		return F_ERR_NOTSUPPSECTORSIZE;
	}

	f_bootrecord->sector_per_cluster=*ptr++;
	ptr=_get16bitl(&f_bootrecord->reserved_sectors,ptr);
	f_bootrecord->number_of_FATs=*ptr++;
	ptr=_get16bitl(&f_bootrecord->max_root_entry,ptr);
	ptr=_get16bitl(&f_bootrecord->number_of_sectors_less32,ptr); /* get sector number <32MB */
	f_bootrecord->media_descriptor=*ptr++;
	ptr=_get16bitl(&f_bootrecord->sector_per_FAT,ptr);
	ptr=_get16bitl(&f_bootrecord->sector_per_Track,ptr);
	ptr=_get16bitl(&f_bootrecord->number_of_heads,ptr);
	ptr=_get32bitl(&f_bootrecord->number_of_hidden_sectors,ptr);

	/* set number of hidden sector acording volume start */
	if (f_bootrecord->number_of_hidden_sectors<vi->sectorstart)
	{
		f_bootrecord->number_of_hidden_sectors=vi->sectorstart;
	}

	ptr=_get32bitl(&f_bootrecord->number_of_sectors,ptr); /* get sector number >32MB */

	if (f_bootrecord->sector_per_FAT)
	{ /* this is 0 on FAT32, but better to check */
		_f_buildsectors(vi);
	}
	else
	{
		if (f_bootrecord->number_of_sectors)
		{
			vi->mediatype=F_FAT32_MEDIA;
		}
		else
		{	/* NTFS sets number_of_sectors also to 0 */
			vi->mediatype=F_UNKNOWN_MEDIA;
			return F_ERR_INVALIDMEDIA;
		}
	}

	if (vi->mediatype==F_FAT32_MEDIA)
	{
		ptr=_get32bitl(&f_bootrecord->sector_per_FAT32,ptr);
		ptr=_get16bitl(&f_bootrecord->extflags,ptr);
		ptr=_get16bitl(&f_bootrecord->fsversion,ptr);
		ptr=_get32bitl(&f_bootrecord->rootcluster,ptr);
		ptr=_get16bitl(&f_bootrecord->fsinfo,ptr);
		ptr=_get16bitl(&f_bootrecord->bkbootsec,ptr);
		ptr=_get8bit(f_bootrecord->reserved,sizeof(f_bootrecord->reserved),ptr);

		ptr=_get16bitl(&f_bootrecord->logical_drive_num,ptr);
		f_bootrecord->extended_signature=*ptr++;
		ptr=_get32bitl(&f_bootrecord->serial_number,ptr);
		ptr=_get8bit(f_bootrecord->volume_name,sizeof(f_bootrecord->volume_name),ptr);
		ptr=_get8bit(f_bootrecord->FAT_name,sizeof(f_bootrecord->FAT_name),ptr);
		ptr=_get8bit(0,448-28,ptr); /* smaller!! */
		ptr=_get8bit(f_bootrecord->executable_marker,sizeof(f_bootrecord->executable_marker),ptr);
	}
	else
	{
		ptr=_get16bitl(&f_bootrecord->logical_drive_num,ptr);
		f_bootrecord->extended_signature=*ptr++;
		ptr=_get32bitl(&f_bootrecord->serial_number,ptr);
		ptr=_get8bit(f_bootrecord->volume_name,sizeof(f_bootrecord->volume_name),ptr);
		ptr=_get8bit(f_bootrecord->FAT_name,sizeof(f_bootrecord->FAT_name),ptr);
		ptr=_get8bit(0,448,ptr);
		ptr=_get8bit(f_bootrecord->executable_marker,sizeof(f_bootrecord->executable_marker),ptr);
	}

	if (f_bootrecord->media_descriptor!=0xf8)   /* fixdrive */
	{
		if (f_bootrecord->media_descriptor!=0xf0) /* removable */
		{
			return F_ERR_NOTFORMATTED; /* no other allowed */
		}
	}

	return F_NO_ERROR;
}

/****************************************************************************
 *
 * _f_writebootrecord
 *
 * writing boot record onto a volume, it uses number of hidden sector variable
 *
 * INPUTS
 *
 * vi - volume pointer
 *
 * RETURNS
 *
 * error code or zero if successful
 *
 ***************************************************************************/

static int _f_writebootrecord(F_VOLUME *vi)
{
	unsigned char *ptr=vi->sectorbuffer;
	F_BOOTRECORD *f_bootrecord=&vi->bootrecord;
	int ret;

	if (vi->mediatype==F_FAT32_MEDIA)
	{ /* write FS_INFO */
		unsigned short a;

		(void)_memset((void *)(vi->sectorbuffer),0,sizeof(vi->sectorbuffer));

		for (a=0; a<f_bootrecord->reserved_sectors; a++)
		{
			ret=_f_writesector(vi,vi->sectorbuffer,f_bootrecord->number_of_hidden_sectors+a,1); /* erase reserved area */
			if (ret) return ret;
		}

		ptr=_set32bitl(0x41615252UL,ptr); /* signature */
		ptr=_set8bitzero(480,ptr);        /* reserved */
		ptr=_set32bitl(0x61417272UL,ptr); /* signature   */
		ptr=_set32bitl(0xffffffffUL,ptr); /* no last */
		ptr=_set32bitl(0xffffffffUL,ptr); /* no hint */
		ptr=_set8bitzero(12,ptr);         /* reserved */
		ptr=_set32bitl(0xaa550000UL,ptr); /* trail */

		ret=_f_writesector(vi,vi->sectorbuffer,f_bootrecord->number_of_hidden_sectors+f_bootrecord->fsinfo,1); /* write FSINFO */
		if (ret) return ret;

		ret=_f_writesector(vi,vi->sectorbuffer,f_bootrecord->number_of_hidden_sectors+f_bootrecord->fsinfo+f_bootrecord->bkbootsec,1); /* write FSINFO */
		if (ret) return ret;
	}


	ptr=vi->sectorbuffer;

	ptr=_set8bit(f_bootrecord->jump_code,sizeof(f_bootrecord->jump_code),ptr);
	ptr=_set8bit(f_bootrecord->OEM_name,sizeof(f_bootrecord->OEM_name),ptr);
	ptr=_set16bitl(f_bootrecord->bytes_per_sector,ptr);
	*ptr++=f_bootrecord->sector_per_cluster;
	ptr=_set16bitl(f_bootrecord->reserved_sectors,ptr);
	*ptr++=f_bootrecord->number_of_FATs;
	ptr=_set16bitl(f_bootrecord->max_root_entry,ptr);
	ptr=_set16bitl(f_bootrecord->number_of_sectors_less32,ptr); /* set sector number <32MB */
	*ptr++=f_bootrecord->media_descriptor;
	ptr=_set16bitl(f_bootrecord->sector_per_FAT,ptr);
	ptr=_set16bitl(f_bootrecord->sector_per_Track,ptr);
	ptr=_set16bitl(f_bootrecord->number_of_heads,ptr);
	ptr=_set32bitl(f_bootrecord->number_of_hidden_sectors,ptr);
	ptr=_set32bitl(f_bootrecord->number_of_sectors,ptr);		 /* set sector number >32MB */

	if (vi->mediatype==F_FAT32_MEDIA)
	{
		ptr=_set32bitl(f_bootrecord->sector_per_FAT32,ptr);
		ptr=_set16bitl(f_bootrecord->extflags,ptr);   /* ? */
		ptr=_set16bitl(f_bootrecord->fsversion,ptr);  /* ? */
		ptr=_set32bitl(f_bootrecord->rootcluster,ptr); /* ? */
		ptr=_set16bitl(f_bootrecord->fsinfo,ptr);      /* ? */
		ptr=_set16bitl(f_bootrecord->bkbootsec,ptr);   /* ? */
		ptr=_set8bit(f_bootrecord->reserved,sizeof(f_bootrecord->reserved),ptr); /* ? */

		ptr=_set16bitl(f_bootrecord->logical_drive_num,ptr);
		*ptr++=f_bootrecord->extended_signature;
		ptr=_set32bitl(f_bootrecord->serial_number,ptr);
		ptr=_set8bit(f_bootrecord->volume_name,sizeof(f_bootrecord->volume_name),ptr);
		ptr=_set8bit(f_bootrecord->FAT_name,sizeof(f_bootrecord->FAT_name),ptr);
		ptr=_set8bit(0,448-28,ptr); /* smaller!! */
		ptr=_set8bit(f_bootrecord->executable_marker,sizeof(f_bootrecord->executable_marker),ptr);

		ret=_f_writesector(vi,vi->sectorbuffer,f_bootrecord->number_of_hidden_sectors+f_bootrecord->bkbootsec,1); /* write backup */
		if (ret) return ret;
	}
	else
	{
		ptr=_set16bitl(f_bootrecord->logical_drive_num,ptr);
		*ptr++=f_bootrecord->extended_signature;
		ptr=_set32bitl(f_bootrecord->serial_number,ptr);
		ptr=_set8bit(f_bootrecord->volume_name,sizeof(f_bootrecord->volume_name),ptr);
		ptr=_set8bit(f_bootrecord->FAT_name,sizeof(f_bootrecord->FAT_name),ptr);
		ptr=_set8bit(0,448,ptr);
		ptr=_set8bit(f_bootrecord->executable_marker,sizeof(f_bootrecord->executable_marker),ptr);
	}

	return _f_writesector(vi,vi->sectorbuffer,f_bootrecord->number_of_hidden_sectors,1); /* write bootrecord */
}

#ifdef FATCACHE_ENABLE

/********************************************************************************
 *
 * _f_fatcache_flush
 * Flush the required amount of cached fat sectors.
 *
 * INPUT:
 *	vi - volume information
 *
 * OUTPUT:
 *	F_NO_ERROR - on success
 *	other - on error
 *
 *******************************************************************************/

int _f_fatcache_flush (F_VOLUME *vi)
{
	unsigned int a;
	unsigned int i,j;

	if (vi->fatcache.N)
	{
		j=0;
		do
		{
			for (i=j;i<vi->fatcache.N && (vi->fatcache.dsc+i)->modified==0;i++)
			  ;
			if (i<vi->fatcache.N)
			{
				for (j=i+1;j<vi->fatcache.N && (vi->fatcache.dsc+j)->modified
					&& (vi->fatcache.dsc+(j-1))->sector+1==(vi->fatcache.dsc+j)->sector;j++)
				  ;
				{
					unsigned long fatsector=(vi->fatcache.dsc+i)->sector;

					for (a=0;a<vi->bootrecord.number_of_FATs;a++)
					{
						int ret=_f_writesector(vi,(vi->fatcache.data)+(i*(vi->bootrecord.bytes_per_sector)),fatsector,(int)(j-i));
						if (ret) return ret;
						fatsector+=vi->firstfat.num;
					}
				}

				for (a=i;a<j;(vi->fatcache.dsc+a++)->modified=0);
			}
		} while (i<vi->fatcache.N);
	}

	vi->fatmodified=0;

	return F_NO_ERROR;
}

/****************************************************************************
 *
 * _f_writefatsector
 *
 * writing fat sector into volume, this function check if fat was modified
 * and writes data to the cache
 *
 * INPUTS
 *
 * vi - volume pointer
 *
 * RETURNS
 *
 * error code or zero if successful
 *
 ***************************************************************************/

int _f_writefatsector(F_VOLUME *vi)
{
	if (!vi->fatmodified)
	{
		return F_NO_ERROR;
	}

	if (vi->fatsector>=vi->firstfat.num) 
	{
		return F_ERR_INVALIDSECTOR;
	}
	else
	{
		unsigned long fatsector=vi->firstfat.sector+vi->fatsector;
		t_FatCache *fatcache = &vi->fatcache;
		t_CacheDsc *cachedsc = fatcache->dsc;
		unsigned int num;

		num=fatcache->N;
		while (num--)
		{
			if (cachedsc->sector == fatsector)
			{
				/* set modified on cache entry */
				cachedsc->modified=1;

				/* remove modified flag from fatsector*/
				vi->fatmodified=0;
				return F_NO_ERROR;
			}

			cachedsc++;
		}

		return F_ERR_NOTFOUND;
	}
}

/****************************************************************************
 *
 * _f_getfatsector
 *
 * read a fat sector from cache/media
 *
 * INPUTS
 *
 * vi - volume pointer
 * sector - which fat sector is needed, this sector number is zero based
 *
 * RETURNS
 *
 * error code or zero if successful
 *
 ***************************************************************************/

static int _f_getfatsector(F_VOLUME *vi,unsigned long sector)
{
	unsigned int i,a;
	unsigned int pos;
	unsigned char *ptr;

	if (vi->fatsector!=sector)
	{
		if (vi->fatmodified)
		{
      		int ret=_f_writefatsector(vi);
			if (ret) return ret;
		}

		if (sector>=vi->firstfat.num)
		{
			return F_ERR_INVALIDSECTOR;
		}

		for (pos=0;pos<vi->fatcache.N && (vi->fatcache.dsc+pos)->sector!=(vi->firstfat.sector+sector);pos++)
		  ;
		if (pos==vi->fatcache.N)
		{
			unsigned long readfrom;
			unsigned int readahead=FATCACHE_READAHEAD*(F_MAX_SECTOR_SIZE/vi->bootrecord.bytes_per_sector);
			unsigned int readnum;

			if (pos==(readahead*FATCACHE_BLOCKS))
			{
				int ret=_f_fatcache_flush(vi);
				if (ret) return ret;
				if (vi->fatcache.Npos==(readahead*FATCACHE_BLOCKS)) vi->fatcache.Npos=0;
				pos=vi->fatcache.Npos;
			}
			vi->fatsector=sector;

			/* align readfrom into cache boundary */
			readfrom=(sector/readahead)*readahead;

			/* check whether number of sectors points still in FAT area */
			if (readfrom+readahead > vi->firstfat.num)
			{
				readnum = (unsigned int)(vi->firstfat.num - readfrom);
			}
			else
			{
				readnum=readahead;
			}

			ptr=(vi->fatcache.data)+(pos*(vi->bootrecord.bytes_per_sector));
			for (a=0; a<vi->bootrecord.number_of_FATs; a++)
			{
				if (!_f_readsector(vi,ptr,(vi->firstfat.sector+readfrom)+(a*vi->firstfat.num),readnum))
				{
					vi->fat=ptr+((sector-readfrom)*(vi->bootrecord.bytes_per_sector));
					for (i=0;i<readahead;i++) (vi->fatcache.dsc+(pos+i))->sector=vi->firstfat.sector+readfrom+(unsigned long)i;
					if (vi->fatcache.N<(readahead*FATCACHE_BLOCKS)) vi->fatcache.N+=readahead;
					vi->fatcache.Npos+=readahead;
					return F_NO_ERROR;
				}
			}

			vi->fatsector=(unsigned long)(-1);
			return F_ERR_READ;
		}
		else
		{
			vi->fatsector=sector;
			vi->fat=(vi->fatcache.data)+(pos*(vi->bootrecord.bytes_per_sector));
		}
	}

	return F_NO_ERROR;
}

#else /* FATCACHE_ENABLE */

/****************************************************************************
 *
 * _f_writefatsector
 *
 * writing fat sector into volume, this function check if fat was modified
 * and writes data
 *
 * INPUTS
 *
 * vi - volume pointer
 *
 * RETURNS
 *
 * error code or zero if successful
 *
 ***************************************************************************/

int _f_writefatsector(F_VOLUME *vi)
{
	if (vi->fatmodified)
	{
		unsigned long fatsector=vi->firstfat.sector+vi->fatsector;
		unsigned char a;

		if (vi->fatsector>=vi->firstfat.num)
		{
			return F_ERR_INVALIDSECTOR;
		}

		for (a=0; a<vi->bootrecord.number_of_FATs; a++)
		{
			int ret=_f_writesector(vi,vi->fat,fatsector,1);
			if (ret) return ret;

			fatsector+=vi->firstfat.num;
		}

		vi->fatmodified=0;
	}

	return F_NO_ERROR;
}

/****************************************************************************
 *
 * _f_getfatsector
 *
 * read a fat sector from media
 *
 * INPUTS
 *
 * vi - volume pointer
 * sector - which fat sector is needed, this sector number is zero based
 *
 * RETURNS
 *
 * error code or zero if successful
 *
 ***************************************************************************/

static int _f_getfatsector(F_VOLUME *vi,unsigned long sector)
{
	if (vi->fatsector!=sector)
	{
		unsigned long fatsector;
		unsigned char a;

		if (vi->fatmodified)
		{
      		int ret=_f_writefatsector(vi);
			if (ret) return ret;
		}

		vi->fatsector=sector;

		if (vi->fatsector>=vi->firstfat.num)
		{
			return F_ERR_INVALIDSECTOR;
		}

		fatsector=vi->firstfat.sector+vi->fatsector;

		for (a=0; a<vi->bootrecord.number_of_FATs; a++)
		{
			if (!_f_readsector(vi,vi->fat,fatsector,1))
			{
				return F_NO_ERROR;
			}
			fatsector+=vi->firstfat.num;
		}
		return F_ERR_READ;
	}

	return F_NO_ERROR;
}

#endif

/****************************************************************************
 *
 * _f_setclustervalue
 *
 * set a cluster value in the FAT
 *
 * INPUTS
 *
 * vi - volume pointer
 * cluster - which cluster's value need to be modified
 * data - new value of the cluster
 *
 * RETURNS
 *
 * error code or zero if successful
 *
 ***************************************************************************/

int _f_setclustervalue(F_VOLUME *vi,unsigned long cluster, unsigned long data)
{
	int ret;

	if (vi->mediatype==F_FAT16_MEDIA)
	{
		unsigned long sector=cluster;
		unsigned short s_data=(unsigned short)(data & 0xffff); /* keep 16 bit only */

		sector/=((vi->bootrecord.bytes_per_sector)/2);
		cluster-=sector*((vi->bootrecord.bytes_per_sector)/2);

		ret=_f_getfatsector(vi,sector);
		if (ret) return ret;

		if (_f_get16bitl(&vi->fat[cluster<<1])!=s_data)
		{
			_f_set16bitl(&vi->fat[cluster<<1],s_data);
			vi->fatmodified=1;
		}
	}
	else if (vi->mediatype==F_FAT12_MEDIA)
	{
		unsigned char f12new[2]={0,0};
		unsigned long sector=cluster;
		unsigned long pos;
		unsigned short s_data=(unsigned short)(data & 0x0fff); /* keep 12 bit only */

		if (cluster&1) s_data <<= 4;

		_f_set16bitl(f12new,s_data); /* create new data */

		sector+=sector/2;		  /* 1.5 bytes */
		pos=sector % (vi->bootrecord.bytes_per_sector);
	    sector/=(vi->bootrecord.bytes_per_sector);

		ret=_f_getfatsector(vi,sector);
		if (ret) return ret;

		if (cluster&1)
		{
			f12new[0] |= vi->fat[pos] & 0x0f;
		}

		if (vi->fat[pos]!=f12new[0])
		{
			vi->fat[pos]=f12new[0];
			vi->fatmodified=1;
		}

		pos++;
		if (pos>=vi->bootrecord.bytes_per_sector)
		{
			ret=_f_getfatsector(vi,sector+1);
			if (ret) return ret;
			pos=0;
		}

		if (!(cluster&1))
		{
			f12new[1] |= vi->fat[pos] & 0xf0;
		}

		if (vi->fat[pos]!=f12new[1])
		{
			vi->fat[pos]=f12new[1];
			vi->fatmodified=1;
		}

		return F_NO_ERROR;
	}
	else if (vi->mediatype==F_FAT32_MEDIA)
	{
		unsigned long sector=cluster;
		unsigned long oldv;

	    sector/=((vi->bootrecord.bytes_per_sector)/4);
		cluster-=sector*((vi->bootrecord.bytes_per_sector)/4);

		ret=_f_getfatsector(vi,sector);
		if (ret) return ret;

		oldv=_f_get32bitl(&vi->fat[cluster<<2]);

		data&=0x0fffffff;
		data|=oldv & 0xf0000000UL; /* keep 4 top bits */

		if (data!=oldv)
		{
			_f_set32bitl(&vi->fat[cluster<<2],data);
			vi->fatmodified=1;
		}

		return F_NO_ERROR;
	}
	else
	{
		return F_ERR_INVALIDMEDIA;
	}

	return F_NO_ERROR;
}

/****************************************************************************
 *
 * _f_getclustervalue
 *
 * get a cluster value from FAT
 *
 * INPUTS
 *
 * vi - volume pointer
 * cluster - which cluster value is requested
 * pvalue - where to store data
 *
 * RETURNS
 *
 * error code or zero if successful
 *
 ***************************************************************************/

int _f_getclustervalue(F_VOLUME *vi,unsigned long cluster,unsigned long *pvalue)
{
	unsigned long val;
	int ret;

	if (vi->mediatype==F_FAT16_MEDIA)
	{
		unsigned long sector=cluster;
	    sector/=((vi->bootrecord.bytes_per_sector)/2);
		cluster-=sector*((vi->bootrecord.bytes_per_sector)/2);

		ret=_f_getfatsector(vi,sector);
		if (ret) return ret;

		val=_f_get16bitl(&(vi->fat[cluster<<1])); /* 2bytes */
		if (val>=0xfff0) val|=0x0ffff000;   /* extends it */

		if (pvalue) *pvalue=val;

		return F_NO_ERROR;
	}
	else if (vi->mediatype==F_FAT12_MEDIA)
	{
		unsigned char dataf12[2]={0,0};
		unsigned long sector=cluster;
		unsigned long pos;

		sector+=sector/2;		  /* 1.5 bytes */
		pos=sector % (vi->bootrecord.bytes_per_sector);
	    sector/=(vi->bootrecord.bytes_per_sector);

		ret=_f_getfatsector(vi,sector);
		if (ret) return ret;

		dataf12[0]=vi->fat[pos++];

		if (pos>=vi->bootrecord.bytes_per_sector)
		{
			ret=_f_getfatsector(vi,sector+1);
			if (ret) return ret;
			pos=0;
		}

		dataf12[1]=vi->fat[pos];

		val=_f_get16bitl(dataf12);

		if (cluster&1)
		{
			val = val >> 4;
		}
		else
		{
			val = val & 0xfff;
		}

		if (val>=0x0ff0) val|=0x0ffff000;   /* extends it */

		if (pvalue) *pvalue=val;

		return F_NO_ERROR;
	}
	else if (vi->mediatype==F_FAT32_MEDIA)
	{
		unsigned long sector=cluster;
	    sector/=((vi->bootrecord.bytes_per_sector)/4);
		cluster-=sector*((vi->bootrecord.bytes_per_sector)/4);

		ret=_f_getfatsector(vi,sector);
		if (ret) return ret;

		if (pvalue) *pvalue=_f_get32bitl(&vi->fat[cluster<<2]) & 0x0fffffff; /* 28bit */

		return F_NO_ERROR;
	}
	else
	{
		return F_ERR_INVALIDMEDIA;
	}
}


/****************************************************************************
 *
 * _f_close_drive_files
 *
 * Close all files that belongs to a drive.
 *
 * INPUTS
 *
 * drivenum - where to close all files
 *
 ***************************************************************************/

static void _f_close_drive_files (int drivenum)
{
	int a;

	for (a=0; a<F_MAXFILES; a++)
	{
		FN_FILEINT *f=&f_filesystem.files[a];

	    if (f->drivenum==drivenum)
		{
			if (f->mode != FN_FILE_CLOSE)
			{
#ifdef USE_MALLOC
				if (f->WrDataCache.pos) 
				{
					__free(f->WrDataCache.pos);
					f->WrDataCache.pos=NULL;
				}
#endif
				/* forced to be closed (signal error on f_close drive is not exist below it)*/
				f->mode=FN_FILE_CLOSE;
				/* syncronized files are always on the same drive */
			}
		}
	}
}

/****************************************************************************
 *
 * _f_getvolume
 *
 * getting back a volume info structure of a given drive, it try to mounts
 * drive if it was not mounted before
 *
 * INPUTS
 *
 * fm - multi structure
 * drivenum - which drive volumeinfo needs to be retreive
 * pvi - pointer of volumeinfo structure where to put the pointer
 *
 * RETURNS
 *
 * error code or zero if successful
 *
 ***************************************************************************/

int _f_getvolume(F_MULTI *fm,int drivenum,F_VOLUME **pvi)
{
	F_VOLUME *vi;
	int ret;

	if (drivenum < 0 || drivenum>=FN_MAXVOLUME) return F_ERR_INVALIDDRIVE;

	vi=&f_filesystem.volumes[drivenum];

	ret=_f_mutex_get(fm,vi);
	if (ret) return ret;

#if USE_TASK_SEPARATED_CWD
	vi->cwd=fm->f_vols[drivenum].cwd; /* safety uses of vi->cwd */
#endif

	if (pvi) *pvi=vi;	/* set volumeinfo */

	switch (vi->state)
	{

	case F_STATE_NONE:
		_f_close_drive_files(drivenum);
		return F_ERR_INVALIDDRIVE;

	case F_STATE_WORKING:
	case F_STATE_WORKING_WP:

		if (!_f_checkstatus(vi,0)) 
		{
			return F_NO_ERROR;
		}

      /*  here we don't stop case flow,   */
      /*  because we have to clean up this volume!  */

	case F_STATE_NEEDMOUNT:
		{
			long drvstate;

			/* check if driver exist */
			if (!vi->driver)
			{
				return F_ERR_INVALIDDRIVE;
			}

#ifdef FATCACHE_ENABLE
			{
			  int i;
			  vi->fatcache.N=0;
			  vi->fatcache.Npos=0;
			  for (i=0;i<FATCACHE_SIZE*(F_MAX_SECTOR_SIZE/F_DEF_SECTOR_SIZE);i++) 
			  {
			    (vi->fatcache.dsc+i)->modified=0;
			  }
			}
#endif
#ifdef DIRCACHE_ENABLE
			vi->dircache_start=(unsigned long)-1;
#endif

#if (!USE_TASK_SEPARATED_CWD)
			vi->cwd[0]=0;
#endif

			vi->fatmodified=0;
			vi->fatsector=(unsigned long)(-1);
			vi->direntrysector=(unsigned long)(-1);
			vi->lastalloccluster=0;
			vi->cspaceok=0;

			_f_close_drive_files(drivenum);

			if (vi->cwd) vi->cwd[0]=0; /* reset cwd */
			vi->mediatype=F_UNKNOWN_MEDIA;

			/* checking the status 1st */
			ret=_f_checkstatus(vi,&drvstate);
			if (ret) return ret;

			/* read boot record */
			ret=_f_readbootrecord(vi);
			if (ret==F_ERR_NOTSUPPSECTORSIZE) return ret;
			if (ret==0)
			{
				_f_buildsectors(vi);

				f_filesystem.drvbldnum++;
				if ((f_filesystem.drvbldnum<<F_MAXFILES_SHIFT)==0UL) f_filesystem.drvbldnum=1;

#ifdef FATBITFIELD_ENABLE
				if (vi->fatbitfield)
				{
					__free(vi->fatbitfield);
					vi->fatbitfield=0;
				}
				if (vi->mediatype==F_FAT32_MEDIA)
				{
					vi->fatbitfield=(unsigned char *)__malloc((vi->firstfat.num + 7) >>3); /* number of sec / 8*/
					if (vi->fatbitfield==NULL) return F_ERR_ALLOCATION;
					(void)_memset(vi->fatbitfield,0,(vi->firstfat.num + 7) >> 3 );
				}
#endif

				if (drvstate & F_ST_WRPROTECT)
				{
					vi->state=F_STATE_WORKING_WP;
				}
				else
				{
					vi->state=F_STATE_WORKING;
				}

				return F_NO_ERROR;
			}

			vi->mediatype=F_UNKNOWN_MEDIA;
			return F_ERR_NOTFORMATTED;
		}

	default: ;
	}

	return F_ERR_INVALIDDRIVE;
}

/****************************************************************************
 *
 * fn_createdriver
 *
 * Creating a driver, it calls driver init function
 *
 * INPUTS
 *
 * fm - multi structure
 * driver - where to store created driver
 * driver_init - driver init function
 * driver_param - parameter to pass to driver init
 *
 * RETURNS
 *
 * error code or zero if successful
 *
 ***************************************************************************/

int fn_createdriver(F_MULTI *fm,F_DRIVER **driver,F_DRIVERINIT driver_init,unsigned long driver_param)
{
	int ret;
	int vcou;

	if (!driver_init)
	{
		return F_SETLASTERROR(F_ERR_INITFUNC);
	}

	/* calling driver init function */
	*driver=driver_init(driver_param);
	if (!(*driver)) return F_SETLASTERROR(F_ERR_INITFUNC);

	/* check if this is a new driver */
	for (vcou=0; vcou<FN_MAXVOLUME; vcou++)
	{
		F_VOLUME *v=&f_filesystem.volumes[vcou];
		if (v->driver==*driver)
		{
			return F_SETLASTERROR(F_ERR_DUPLICATED);	/* a volume uses the same driver */
		}
	}

	/* creating mutex */
	ret=f_mutex_create(&(*driver)->mutex);
	if (ret) return F_SETLASTERROR(F_ERR_INITFUNC);

	/* set as separated driver */
	(*driver)->separated=1;

	return F_SETLASTERROR(F_NO_ERROR);
}

/****************************************************************************
 *
 * fn_initvolume
 *
 * initiate a volume, this function has to be called 1st to set physical
 * driver function to a given volume
 *
 * INPUTS
 *
 * fm - multi structure
 * drvnumber - which drive need to be initialized
 * driver_init - driver init function
 * driver_param - parameter to pass to driver init
 *
 * RETURNS
 *
 * error code or zero if successful
 *
 ***************************************************************************/

int fn_initvolume(F_MULTI *fm,int drvnumber,F_DRIVERINIT driver_init,unsigned long driver_param)
{
	F_VOLUME *vi;
	F_DRIVER *driver;
	int ret;

	if (drvnumber<0 || drvnumber>=FN_MAXVOLUME) return F_SETLASTERROR(F_ERR_INVALIDDRIVE);
	vi=&f_filesystem.volumes[drvnumber];
	if (vi->state!=F_STATE_NONE) return F_SETLASTERROR(F_ERR_DRVALREADYMNT);

	if (fn_createdriver(fm,&driver,driver_init,driver_param))
	{
		return F_SETLASTERROR(F_ERR_INITFUNC);
	}

	/* check if driver exist */
	if (!driver)
	{
		return F_SETLASTERROR(F_ERR_INVALIDDRIVE);
	}

	/* driver is not separated */
	driver->separated=0;

	/* init volume as partition 0 as default partition */
	ret=fn_initvolumepartition(fm,drvnumber,driver,0);
	if (ret) return F_SETLASTERROR(ret);

	return F_SETLASTERROR(F_NO_ERROR);
}

/****************************************************************************
 *
 * fn_releasedriver
 *
 * releasing a driver
 *
 * INPUTS
 *
 * fm - multi structure pointer
 * driver - driver structure
 *
 * RETURNS
 *
 * errorcode
 *
 ***************************************************************************/

int fn_releasedriver(F_MULTI *fm,F_DRIVER *driver)
{
	if (driver)
	{
		int drvcou;

		/* delete all volumes belongs to this driver */
		for (drvcou=0; drvcou<FN_MAXVOLUME; drvcou++)
		{
			F_VOLUME *vi=&f_filesystem.volumes[drvcou];
			if (vi->driver==driver)
			{
				int ret=fn_delvolume(fm,drvcou);
				if (ret) return F_SETLASTERROR(ret);

				/* detach driver */
				vi->driver=0;
			}
		}

		/* delete mutex */
		(void)f_mutex_delete(&driver->mutex);

		/* remove reference from fm */
		fm->pmutex=0;

		/* releasing the driver */
		if (driver->release)
		{
			driver->release(driver);
		}
	}

	return F_SETLASTERROR(F_NO_ERROR);
}

/****************************************************************************
 *
 * fn_initvolumepartition
 *
 * initiate a volume, this function has to be called 1st to set physical
 * driver function to a given volume and a given partition
 *
 * INPUTS
 *
 * fm - multi structure
 * drvnumber - which drive need to be initialized
 * driver - driver to be used for this volume
 * partition - selected partition on the drive
 *
 * RETURNS
 *
 * error code or zero if successful
 *
 ***************************************************************************/

int fn_initvolumepartition(F_MULTI *fm,int drvnumber,F_DRIVER *driver, int partition)
{
	F_VOLUME *vi;
	int ret;

	if (drvnumber<0 || drvnumber>=FN_MAXVOLUME) return F_SETLASTERROR(F_ERR_INVALIDDRIVE);

	vi=&f_filesystem.volumes[drvnumber];
	if (vi->state!=F_STATE_NONE) return F_SETLASTERROR(F_ERR_DRVALREADYMNT);

#ifdef FATCACHE_ENABLE
  #ifdef USE_MALLOC
	vi->fatcache.dsc=(t_CacheDsc *)__malloc(sizeof(t_CacheDsc)*FATCACHE_SIZE*(F_MAX_SECTOR_SIZE/F_DEF_SECTOR_SIZE));
	if (vi->fatcache.dsc==NULL) return F_SETLASTERROR(F_ERR_ALLOCATION);

	vi->fatcache.data=(unsigned char *)__malloc(FATCACHE_SIZE*F_MAX_SECTOR_SIZE);
	if (vi->fatcache.data==NULL)
	{
		__free(vi->fatcache.dsc);
		return F_SETLASTERROR(F_ERR_ALLOCATION);
	}
  #else
	vi->fatcache.dsc=&(vi->fatcache.dsc_array[0]);
  #endif
#endif

#if F_LONGFILENAME
#ifdef DIRCACHE_ENABLE
#ifdef USE_MALLOC
	vi->dircache=(unsigned char *)__malloc(DIRCACHE_SIZE*F_MAX_SECTOR_SIZE);
	if (vi->dircache==NULL)
	{
  #ifdef FATCACHE_ENABLE
		__free(vi->fatcache.dsc);
		__free(vi->fatcache.data);
		return F_SETLASTERROR(F_ERR_ALLOCATION);
  #endif
	}

#endif
#endif
#endif

	vi->state=F_STATE_NONE;
	vi->partition=partition;	/* set partition */
	vi->driver=driver;			/* set driver to the volume */

#ifdef FATBITFIELD_ENABLE
	vi->fatbitfield=0;
#endif

	/* remove changed bit on removable devices */
	(void)_f_checkstatus(vi,0);
		
	vi->state=F_STATE_NEEDMOUNT;

	ret=_f_getvolume(fm,drvnumber,0);

/*	if (!ret && f_filesystem.volumes[fm->curdrive].state==F_STATE_NONE) fm->curdrive=drvnumber; */

	return F_SETLASTERROR(ret);
}

/****************************************************************************
 *
 * fn_checkvolume
 *
 * Checking a volume about its state
 *
 * INPUTS
 *
 * fm - multi structure
 * drvnumber - number of drive to check
 *
 * RETURNS
 *
 * error code or zero if successful
 *
 ***************************************************************************/

int fn_checkvolume(F_MULTI *fm,int drvnumber)
{
	return F_SETLASTERROR(_f_getvolume(fm,drvnumber,0));
}

/****************************************************************************
 *
 * fn_delvolume
 *
 * Deletes a previously initialized volume.
 *
 * INPUTS
 *
 * fm - multi structure
 * drvnumber - number of drive to delete
 *
 * RETURNS
 *
 * errorcode
 *
 ***************************************************************************/

int fn_delvolume (F_MULTI *fm, int drvnumber)
{
	int rc=F_NO_ERROR;
	F_VOLUME *vi;

	if (drvnumber<0 || drvnumber>=FN_MAXVOLUME)
	{
		rc=F_ERR_INVALIDDRIVE;
	}
	else
	{
		vi=&f_filesystem.volumes[drvnumber];

		rc=_f_mutex_get(fm,vi);
		if (rc) return F_SETLASTERROR(rc);

		if (vi->state!=F_STATE_NONE)
		{
			/* close files belong to this drive */
			_f_close_drive_files(drvnumber);

			/* set status back to none, volume is not working from this point */
			vi->state=F_STATE_NONE;

#ifdef FATCACHE_ENABLE
			rc=_f_fatcache_flush(vi);
			if (rc) return F_SETLASTERROR(rc);
#endif
			if (vi->driver)
			{
				F_DRIVER *driver=vi->driver;

				/* simple detach driver */
				vi->driver=0;

				/* if driver is not separated from volume, then release it as well */
				if (!driver->separated)
				{
					if (fn_releasedriver(fm,driver))
					{
						return F_SETLASTERROR(F_ERR_INITFUNC);
					}
				}
			}

#ifdef USE_MALLOC

#ifdef FATCACHE_ENABLE
			__free(vi->fatcache.data);
			__free(vi->fatcache.dsc);
#endif

#if F_LONGFILENAME
#ifdef DIRCACHE_ENABLE
			__free(vi->dircache);
#endif
#endif

#endif

#ifdef FATBITFIELD_ENABLE
			if (vi->fatbitfield)
			{
				__free(vi->fatbitfield);
				vi->fatbitfield=0;
			}
#endif
		}
	}

	return F_SETLASTERROR(rc);
}

/****************************************************************************
 *
 * fn_get_volume_count
 *
 * Returns the number of mounted volumes
 *
 * INPUTS
 *
 * fm - multi structure
 *
 * RETURNS
 *
 * number of mounted volumes
 *
 ***************************************************************************/

int fn_get_volume_count (F_MULTI *fm)
{
	int rc=0;
	int i;
	F_VOLUME *vi;

	for (i=0;i<FN_MAXVOLUME;i++)
	{
		vi=&f_filesystem.volumes[i];
		if (vi->state!=F_STATE_NONE) ++rc;
	}

	F_SETLASTERROR_NORET(F_NO_ERROR);
	return rc;
}

/****************************************************************************
 *
 * fn_get_volume_list
 *
 * get active volume list. It puts into the given buffer the active volumes
 * number
 *
 * INPUTS
 *
 * fm - multi structure
 * buf - where function puts active volumes number,
 *		 buf size must be at least FN_MAXVOLUME	integers
 *
 * RETURNS
 *
 * number of mounted volumes
 *
 ***************************************************************************/

int fn_get_volume_list (F_MULTI *fm,int *buf)
{
	int rc=0;
	int i;
	F_VOLUME *vi;

	for (i=0;i<FN_MAXVOLUME;i++)
	{
		vi=&f_filesystem.volumes[i];
		if (vi->state!=F_STATE_NONE)
		{
			++rc;
			*buf++=i;
		}
	}

	F_SETLASTERROR_NORET(F_NO_ERROR);
	return rc;
}

/****************************************************************************
 *
 * _f_clustertopos
 *
 * convert a cluster position into physical sector position
 *
 * INPUTS
 *
 * vi - volume pointer
 * cluster - original cluster position
 * pos - position structure to fills the position
 *
 ***************************************************************************/

void _f_clustertopos(const F_VOLUME *vi,unsigned long cluster, F_POS *pos)
{
	pos->prevcluster=pos->cluster;
 	pos->cluster=cluster;

	if (!cluster)
	{
		unsigned long sectorcou=vi->root.num;
		pos->sector=vi->root.sector;
		pos->sectorend=pos->sector+sectorcou;
	}
	else
	{
		pos->sector=_getsectorfromcluster(vi,pos->cluster);
		pos->sectorend=pos->sector+vi->bootrecord.sector_per_cluster;
	}

	/* if invalid cluster then disable it to use */
	if (cluster>=F_CLUSTER_RESERVED)
	{
		pos->sectorend=0;
	}

	pos->sectorbegin=pos->sector;

	pos->pos=0;
}


/****************************************************************************
 *
 * _f_prepareformat
 *
 * preparing boot record for formatting, it sets and calculates values
 *
 * INPUTS
 *
 * phy - media physical descriptor
 * f_bootrecord - which bootrecord need to be prepare
 * number_of_hidden_sectors - where boot record starts
 * fattype - one of this definitions F_FAT12_MEDIA,F_FAT16_MEDIA,F_FAT32_MEDIA
 *
 * RETURNS
 *
 * error code or zero if successful
 *
 ***************************************************************************/

static int _f_prepareformat(const F_VOLUME *vi,F_BOOTRECORD *f_bootrecord,long fattype)
{
	if (!vi->sectornum) 
	{
		return F_ERR_INVALIDSECTOR;
	}

	f_bootrecord->jump_code[0]=0xeb;  /* standard jump instruction at start! */
	f_bootrecord->jump_code[1]=0x3c;
	f_bootrecord->jump_code[2]=0x90;

	(void)_memcpy(f_bootrecord->OEM_name,OEM_NAME,8); /* Set OEM_NAME for Media */
	if (vi->phy.bytes_per_sector>F_MAX_SECTOR_SIZE)
	{
		return F_ERR_NOTSUPPSECTORSIZE;
	}

	f_bootrecord->bytes_per_sector=vi->phy.bytes_per_sector;	  /* force setting bytes per sector for the Media */

	f_bootrecord->number_of_FATs=2;	   /* minimum 2 Fat for safety reason */

	if (fattype!=F_FAT32_MEDIA)
	{
		f_bootrecord->reserved_sectors=1;  /* reserved for boot record */

		if (vi->sectornum<=64)
		{
	   		f_bootrecord->max_root_entry=64;  /* maximum number of directory entry in root */
		}
		else if (vi->sectornum<=128)
		{
	   		f_bootrecord->max_root_entry=128;  /* maximum number of directory entry in root */
		}
		else
		{
	   		f_bootrecord->max_root_entry=512;  /* maximum number of directory entry in root */
		}
	}
	else
	{
		f_bootrecord->reserved_sectors=32+4; /* reserve some sector at start */
   		f_bootrecord->max_root_entry=0;	   /* in fat32 root directory is chained as subdirectory! */
	}

	f_bootrecord->media_descriptor=vi->phy.media_descriptor; /* set media descriptor */

	f_bootrecord->sector_per_Track=vi->phy.sector_per_track;
	f_bootrecord->number_of_heads=vi->phy.number_of_heads;

	f_bootrecord->number_of_hidden_sectors=vi->sectorstart; /* if this is not partioned */
	f_bootrecord->logical_drive_num=0;

	f_bootrecord->extended_signature=0x29;

 	(void)_memcpy(f_bootrecord->volume_name,"NO LABEL   ",11); /* Set NO LABEL for media */

	f_bootrecord->executable_marker[0]=0x55;   /* set sector end */
	f_bootrecord->executable_marker[1]=0xaa;

	f_bootrecord->serial_number=f_getrand(f_bootrecord->serial_number^vi->sectornum); /* Serial number is a random number for Media*/


	if (vi->sectornum<0x10000UL && fattype!=F_FAT32_MEDIA)
	{
		/* check if number of sectors fit in 16 bits */
		f_bootrecord->number_of_sectors_less32=(unsigned short)(vi->sectornum);
		f_bootrecord->number_of_sectors=0;
	}
	else
	{
		/* This is also default on FAT32 */
		f_bootrecord->number_of_sectors_less32=0;
		f_bootrecord->number_of_sectors=vi->sectornum;
	}

	if (fattype==F_FAT12_MEDIA)
	{
		f_bootrecord->sector_per_cluster=1;
   		while (f_bootrecord->sector_per_cluster)
		{ /* try FAT12 */
   			if (vi->sectornum / f_bootrecord->sector_per_cluster < F_FAT12_MAX_CLUSTER) break;
   			f_bootrecord->sector_per_cluster<<=1;
   		}
   		if (!f_bootrecord->sector_per_cluster) return F_ERR_MEDIATOOLARGE; /* fat12 cannot be there */
	}
	else if (fattype==F_FAT16_MEDIA)
	{
   		f_bootrecord->sector_per_cluster=1;
   		while (f_bootrecord->sector_per_cluster)
		{ /* try FAT16 */
   			if (vi->sectornum / f_bootrecord->sector_per_cluster < F_FAT16_MAX_CLUSTER) break;
   			f_bootrecord->sector_per_cluster<<=1;
   		}
   		if (!f_bootrecord->sector_per_cluster) return F_ERR_MEDIATOOLARGE; /* fat16 cannot be there */
	}
	else if (fattype==F_FAT32_MEDIA)
	{
		unsigned int i;
		for (i=0;i<(sizeof(FAT32_CS)/sizeof(t_FAT32_CS))-1 && vi->sectornum>FAT32_CS[i].max_sectors;i++)
		  ;
		f_bootrecord->sector_per_cluster=FAT32_CS[i].sector_per_cluster;
	}
	else return F_ERR_INVFATTYPE;

	if (f_bootrecord->sector_per_cluster)
	{
		long secpercl=f_bootrecord->sector_per_cluster;
		long nfat=f_bootrecord->number_of_FATs;
		unsigned long roots;
		long fatsec;

   		roots=f_bootrecord->max_root_entry;
	   	roots*=sizeof(F_DIRENTRY);
   		roots/=vi->phy.bytes_per_sector;

		if (fattype==F_FAT12_MEDIA)
		{
   			fatsec=(long)(3*(((vi->sectornum-f_bootrecord->reserved_sectors)-roots)+2*secpercl));
			fatsec+=(1024*secpercl+3*nfat-1);
			fatsec/=(1024*secpercl+3*nfat);

   			f_bootrecord->sector_per_FAT=(unsigned short)(fatsec);

			/* align sectornumber to 4 */
			f_bootrecord->sector_per_FAT+=3;
			f_bootrecord->sector_per_FAT&=~0x3;
		}
		else if (fattype==F_FAT16_MEDIA)
		{
   			fatsec=(long)(((vi->sectornum-f_bootrecord->reserved_sectors)-roots)+2*secpercl);
			fatsec+=(256*secpercl+nfat-1);
			fatsec/=(256*secpercl+nfat);

   			f_bootrecord->sector_per_FAT=(unsigned short)(fatsec);

			/* align sectornumber to 4 */
			f_bootrecord->sector_per_FAT+=3;
			f_bootrecord->sector_per_FAT&=~0x3;
		}
		else if (fattype==F_FAT32_MEDIA)
		{
   			fatsec=(long)((vi->sectornum-f_bootrecord->reserved_sectors)+2*secpercl);
			fatsec+=(128*secpercl+nfat-1);
			fatsec/=(128*secpercl+nfat);

			f_bootrecord->sector_per_FAT32=(unsigned long)fatsec;
			f_bootrecord->sector_per_FAT=0;

			f_bootrecord->extflags=0;
   			f_bootrecord->fsversion=0;
	   		f_bootrecord->rootcluster=2;
   			f_bootrecord->fsinfo=1;
   			f_bootrecord->bkbootsec=6;

   			(void)_memset((void *)(f_bootrecord->reserved),0,sizeof(f_bootrecord->reserved));

			/* align sectornumber to 4 */
			f_bootrecord->sector_per_FAT32+=3;
			f_bootrecord->sector_per_FAT32&=~0x3;
		}

		/* align Data start at 8K or cluster size align */
		{
			unsigned long clustersize;
			unsigned long datasector;
			unsigned long remaining;
			
			clustersize=f_bootrecord->sector_per_cluster;
			clustersize*=f_bootrecord->bytes_per_sector;

			if (clustersize<8192)
			{
				clustersize=8192;
			}

			clustersize/=f_bootrecord->bytes_per_sector;

			/* calculate datsector start */
			datasector=f_bootrecord->reserved_sectors;
			datasector+=f_bootrecord->number_of_hidden_sectors;

			if (fattype==F_FAT32_MEDIA)
			{
				unsigned long fatsectors;

				/* number if fatsectors */
				fatsectors=f_bootrecord->sector_per_FAT32;
				fatsectors *= f_bootrecord->number_of_FATs;

				/* calculate data sector start */
				datasector+=fatsectors;
			}
			else
			{
				unsigned long fatsectors;
				unsigned long rootsectors;

				/* number if fatsectors */
				fatsectors=f_bootrecord->sector_per_FAT;
				fatsectors *= f_bootrecord->number_of_FATs;

				/* calculating number of sectors of root directory */
				rootsectors=f_bootrecord->max_root_entry;
				rootsectors*=sizeof(F_DIRENTRY);
				rootsectors/=f_bootrecord->bytes_per_sector;

				/* calculate data sector start */
				datasector+=fatsectors;
				datasector+=rootsectors;
			}

			/* calculate reserved sector shifting */
			remaining =  datasector - (datasector / clustersize) * clustersize;

			if (remaining)
			{
				remaining = clustersize-remaining;
				f_bootrecord->reserved_sectors += (unsigned short)remaining;
			}
		}

		return F_NO_ERROR;

	}

	return F_ERR_INVALIDMEDIA;
}


/****************************************************************************
 *
 * _f_resetfatentries
 *
 * reset fat entries and write them
 *
 * INPUTS
 *
 * vi - volume pointer
 *
 * RETURNS
 *
 * error code or zero if successful
 *
 ***************************************************************************/

static int _f_resetfatentries(F_VOLUME *vi)
{
	unsigned char *ptr=vi->sectorbuffer;
	unsigned char cou;
	unsigned long sector;
	unsigned long seccou;

	/* reset all bytes */
	(void)_memset((void *)ptr,0,F_MAX_SECTOR_SIZE);

	/* The 1st 2 entries in the FAT is always reserved */
	/* put media descriptor to the 1st byte */
    *ptr++=vi->bootrecord.media_descriptor;

	if (vi->mediatype==F_FAT12_MEDIA)
	{
		*ptr++=0xff;
		*ptr++=0xff;	/* fills 2 entries (2x12bit)*/
	}
	else if (vi->mediatype==F_FAT16_MEDIA)
	{
		*ptr++=0xff;
		*ptr++=0xff;
		*ptr++=0xff;	/* fills 2 entries (2x16bit)*/
	}
	else if (vi->mediatype==F_FAT32_MEDIA)
	{
		*ptr++=0xff;
		*ptr++=0xff;
		*ptr++=0xff;

		*ptr++=0xff;
		*ptr++=0xff;
		*ptr++=0xff;
		*ptr++=0xff;	/* fills 2 entries (2x32bit)*/

		*ptr++=0xf8;
		*ptr++=0xff;
		*ptr++=0xff;
		*ptr++=0x0f;	/* Root directory entry */

	}

	/* set back ptr to the buffer */
	ptr=vi->sectorbuffer;

	/* get the 1st FAT 1st sector*/
	sector=vi->firstfat.sector;

	/* write 1st entries of the FAT */
	for (cou=0; cou<vi->bootrecord.number_of_FATs; cou++)
	{
		int ret=_f_writesector(vi,ptr,sector,1);

		if (ret)
		{
			return ret;
		}

		sector+=vi->firstfat.num;
	}

	/* write other FAT sectors if needed */
	if (vi->firstfat.num)
	{
		/* reset all bytes */
	    (void)_memset(ptr,0,F_MAX_SECTOR_SIZE);

		sector=vi->firstfat.sector;	/* get the 1st FAT 1st sector*/

	    for (cou=0;cou<vi->bootrecord.number_of_FATs; cou++)
		{
			sector++;					 /* 1st FAT sector is already written */
			seccou=vi->firstfat.num-1;	 /* decreased by 1 */

	        while (seccou--)
			{
			   if (_f_writesector(vi,ptr,sector++,1)) return F_ERR_ONDRIVE;
		    }
		}
	}

	return F_NO_ERROR;
}

/****************************************************************************
 *
 * _f_resetrootdir
 *
 * reset root directory entries and write them
 *
 * INPUTS
 *
 * vi - volume pointer
 *
 * RETURNS
 *
 * error code or zero if successful
 *
 ***************************************************************************/

static int _f_resetrootdir(F_VOLUME *vi)
{
	unsigned char *ptr=vi->sectorbuffer;
	unsigned long sector,cou;

	/* reset all entries */
	(void)_memset(ptr,0,F_MAX_SECTOR_SIZE);

	/* write all root directory sectors */
	sector=vi->root.sector;
	cou=vi->root.num;
 	while (cou--)
	{ /* reset root direntries */
  		int ret = _f_writesector(vi,ptr,sector++,1);
  		if (ret) return ret;
	}

	return F_NO_ERROR;
}

/****************************************************************************
 *
 * _f_postformat
 *
 * erase fats, erase root directory, reset variables after formatting
 *
 * INPUTS
 *
 * vi - volume pointer
 * fattype - one of this definitions F_FAT12_MEDIA,F_FAT16_MEDIA,F_FAT32_MEDIA
 *
 * RETURNS
 *
 * error code or zero if successful
 *
 ***************************************************************************/

static int _f_postformat(F_VOLUME *vi,long fattype)
{
	int ret;

	_f_buildsectors(vi); /* get sector positions */

	/* check if data sector is in valid range*/
	if (vi->data.sector >= vi->sectornum)
	{
		return F_ERR_MEDIATOOSMALL;
	}

	/* check if selected fattype is the proper*/
	if (vi->mediatype!=fattype) 
	{
		return F_ERR_MEDIATOOSMALL;
	}

	/* set FAT name according to FAT type */
	if (vi->mediatype==F_FAT12_MEDIA)
	{
		(void)_memcpy(vi->bootrecord.FAT_name,"FAT12   ",8);
	}
	else if (vi->mediatype==F_FAT16_MEDIA)
	{
		(void)_memcpy(vi->bootrecord.FAT_name,"FAT16   ",8);
	}
	else if (vi->mediatype==F_FAT32_MEDIA)
	{
		(void)_memcpy(vi->bootrecord.FAT_name,"FAT32   ",8);
	}
	else return F_ERR_INVALIDMEDIA;

	/* reset variables */
	vi->fatmodified=0;
	vi->fatsector=(unsigned long)(-1);
	vi->direntrysector=(unsigned long)(-1);

	ret=_f_resetfatentries(vi);
	if (ret) return ret;

	ret=_f_resetrootdir(vi);
	if (ret) return ret;

	return _f_writebootrecord(vi);
}

/****************************************************************************
 *
 * fn_createpartition
 *
 * creating a partition(s) on drive
 *
 * INPUTS
 *
 * fm - multistructure
 * driver - driver structure
 * parnum - number of partition in par parameter
 * par - F_PARTITION structure, partition descriptor
 *
 * RETURNS
 *
 * error code or zero if successful
 *
 ***************************************************************************/

int fn_createpartition(F_MULTI *fm,F_DRIVER *driver,int parnum,const F_PARTITION *par)
{
	unsigned char sectorbuffer[F_MAX_SECTOR_SIZE];
	unsigned long mbrsector=0;
	unsigned long sectornum;
	unsigned long partstart=F_SPACE_AFTER_MBR;
	unsigned long extpartstart=0;
	unsigned long prevextstart=0;

	F_PHY phy;
	int parcou;
	int ret;
	unsigned char extpar=0;

	/* check wether getstatus function is exist */
	if (driver->getstatus)
	{
		long status=driver->getstatus(driver);

		/* check write protect flasg */
		if (status & F_ST_WRPROTECT)
		{
			return F_SETLASTERROR(F_ERR_WRITEPROTECT);
		}
	}

	/* get its physical */
	ret=_f_drvgetphy(driver,&phy);
	if (ret) return F_SETLASTERROR(ret);

	/* setting up MBR */
	sectornum=phy.number_of_sectors;

	/* check if it is fit */
	if (sectornum<F_SPACE_AFTER_MBR)
	{
		return F_SETLASTERROR(F_ERR_MEDIATOOSMALL);
	}
	sectornum-=F_SPACE_AFTER_MBR;

	parcou=0;

	while (parnum)
	{
		unsigned char *ptr=sectorbuffer;

		/* write zeroes before partition table */
		ptr=_set8bitzero(0x1be,ptr);

        /* Write partition table */
		for (parcou=0; parcou<4;parcou++)
		{
			/* create empty BR or MBDR */
			sectorbuffer[0]=0;
			sectorbuffer[1]=0;
			sectorbuffer[0x1fe]=0;
			sectorbuffer[0x1ff]=0;

			/* 1st 2 bytes are zero and last byte is not 0x55aa that is ok */
			/* erase or write 1st sector into the new partition */
			ret=_f_drvwritesector(driver,sectorbuffer,extpartstart+partstart);
			if (ret) return F_SETLASTERROR(ret);

			if ((parcou==3) || (extpar && (parcou==1)))
			{
				/* only 3 partition + an extended is fit in primary */
				/* then only 1 partition + an extended is fit in extended */

				/* create extended partition */
				*ptr++=(unsigned char)0x0; /* non active partition */
				*ptr++=(unsigned char)0xff; /* START H*/
				*ptr++=(unsigned char)0xff; /* START S*/
				*ptr++=(unsigned char)0xff; /* START C*/
				*ptr++=F_SYSIND_EXTDOS; /* SI */
				*ptr++=(unsigned char)0xff; /* END H*/
				*ptr++=(unsigned char)0xff; /* END S*/
				*ptr++=(unsigned char)0xff; /* END C*/
				ptr=_set32bitl(partstart+prevextstart,ptr);	/* Start Sector */

				if (!extpar)
				{
					ptr=_set32bitl(sectornum,ptr);	/* Num of Sectors */
				}
				else
				{
					ptr=_set32bitl(par->secnum+F_SPACE_AFTER_MBR,ptr);	/* Next partition size */
					prevextstart+=partstart;
				}

				/* check if it is fit */
				if (sectornum<F_SPACE_AFTER_MBR)
				{
					return F_SETLASTERROR(F_ERR_MEDIATOOSMALL);
				}
				sectornum-=F_SPACE_AFTER_MBR;

				extpartstart+=partstart;	/* increase extended partition start */
				extpar=1;
				partstart=F_SPACE_AFTER_MBR;	/* reset partition start */

				break; /* no more entry after extended partition */
			}
			else
			{
				*ptr++=(unsigned char)0x0; /* non active partition <0x80 means bootable> */
				*ptr++=(unsigned char)0xff; /* START H*/
				*ptr++=(unsigned char)0xff; /* START S*/
				*ptr++=(unsigned char)0xff; /* START C*/
				*ptr++=par->system_indicator; /* SI */
				*ptr++=(unsigned char)0xff; /* END H*/
				*ptr++=(unsigned char)0xff; /* END S*/
				*ptr++=(unsigned char)0xff; /* END C*/
				ptr=_set32bitl(partstart,ptr); 	/* Start Sector */
				ptr=_set32bitl(par->secnum,ptr);	/* Num of Sectors */

				partstart+=par->secnum;		/* goto next partition */

				/* check if it is fit */
				if (sectornum<par->secnum)
				{
					return F_SETLASTERROR(F_ERR_MEDIATOOSMALL);
				}

				par++;	/* goto next partition index */

				parnum--;  /* decrease requested partition number */
				if (!parnum) break; /* if no more partition then stop */
			}
		}

		/* fills remaining if any with zeroes */
		for (parcou++; parcou<4; parcou++)
		{
			ptr=_set8bitzero(0x10,ptr);
		}

		/* sector trail */
		ptr=_set16bitl(0xaa55,ptr);

		/* write this master boot record */
		ret=_f_drvwritesector(driver,sectorbuffer,mbrsector);
		if (ret) return F_SETLASTERROR(ret);

		mbrsector=extpartstart+partstart-F_SPACE_AFTER_MBR;	/* update new position */
	}

	return F_SETLASTERROR(F_NO_ERROR);
}

/****************************************************************************
 *
 * fn_format
 *
 * format a media, 1st it checks existing formatting, then master boot record,
 * then media physical
 *
 * INPUTS
 *
 * fm - multi structure pointer
 * drivenum - which drive format is needed
 * fattype - one of this definitions F_FAT12_MEDIA,F_FAT16_MEDIA,F_FAT32_MEDIA
 *
 * RETURNS
 *
 * error code or zero if successful
 *
 ***************************************************************************/

int fn_format(F_MULTI *fm,int drivenum,long fattype)
{
	F_VOLUME *vi;
	int ret;

	ret=_f_getvolume(fm,drivenum,&vi);
	if (ret && ret!=F_ERR_NOTFORMATTED) return F_SETLASTERROR(ret);

	if (vi->state == F_STATE_WORKING_WP)
	{
		return F_SETLASTERROR(F_ERR_WRITEPROTECT);
	}

	vi->state=F_STATE_NEEDMOUNT;

	ret=_f_prepareformat(vi,&vi->bootrecord,fattype); /* no partition */
	if (ret) return F_SETLASTERROR(ret);

	ret=_f_postformat(vi,fattype);
	return F_SETLASTERROR(ret);
}

/****************************************************************************
 *
 * _f_getcurrsector
 *
 * read current sector according in file structure
 *
 * INPUTS
 *
 * vi - volume pointer
 * f - internal file pointer
 * ptr - where to store sector
 * cnt - number of sectors
 *
 * RETURNS
 *
 * error code or zero if successful
 *
 ***************************************************************************/

int _f_getcurrsector(F_VOLUME *vi,FN_FILEINT *f,char *ptr,unsigned int *pcnt)
{
	int ret;
	unsigned long cluster;
	unsigned int N,cnt=0;

	if (pcnt)
	{
		/* get cnt */
		cnt=*pcnt;

		/* zero read sectors */
		*pcnt=0;
	}
	
	while (f->pos.cluster>=2 && f->pos.cluster<F_CLUSTER_RESERVED)
	{
		if (f->pos.sector<f->pos.sectorend)
		{
			unsigned long fsector=f->pos.sector;

			if (!ptr)
			{
				ptr=(char *)f->data;
				N=1;
			}
			else
			{
				if (cnt>256) cnt=256;
				if ((int)(cnt)>(int)(f->pos.sectorend-f->pos.sector))
				{
					unsigned long lsector=f->pos.sectorend;
					unsigned long spc=vi->bootrecord.sector_per_cluster;
					F_POS pos;

					N=(unsigned int)(f->pos.sectorend-f->pos.sector);
					f->pos.sector=lsector-1;
					pos=f->pos;

#if F_MAXSEEKPOS
					_fn_updateseekpos(f);
#endif

					f->abspos+=(N-1UL)*(vi->bootrecord.bytes_per_sector);

					for (;;)
					{
						ret=_f_getclustervalue(vi,f->pos.cluster,&cluster);
						if (ret) return ret;
						if (cluster>=2 && cluster<F_CLUSTER_RESERVED)
						{
							_f_clustertopos(vi,cluster,&pos);
#if F_MAXSEEKPOS
							_fn_updateseekpos(f);
#endif
							if (pos.sector==lsector)	/* prev.cluster last sector+1==curr.cluster first sector */
							{
								f->pos=pos;
								if (N+spc>=cnt)		/* desired amount of sectors achieved */
								{
									f->pos.sector+=((unsigned long)(cnt)-N)-1UL;
									f->abspos+=((unsigned long)(cnt)-N)*(vi->bootrecord.bytes_per_sector);
									N=cnt;
									break;
								}
								else				/* need more sectors */
								{
									f->abspos+=spc*(vi->bootrecord.bytes_per_sector);
									N+=(unsigned int)spc;
									lsector=pos.sectorend;
									f->pos.sector=lsector-1;
								}
							}
							else
							{
								break;			/* sectors are not consecutive */
							}
						}
						else 				/* next cluster is last cluster */
						{
							f->pos.sector=lsector-1;
							break;
						}
					}
					cnt=N;
				}
				else
				{
#if F_MAXSEEKPOS
					_fn_updateseekpos(f);
#endif
					N=cnt;
					f->pos.sector+=(unsigned long)N-1;
					f->abspos+=(N-1UL)*(vi->bootrecord.bytes_per_sector);
				}
			}

			ret=_f_readsector(vi,ptr,fsector,(int)N);
			if (!ret)
			{
				/* set back read sectors only when read was success */
				if (pcnt)
				{
					*pcnt=N;
				}
			}
			return ret;
		}

		ret=_f_getclustervalue(vi,f->pos.cluster,&cluster);
		if (ret) return ret;

		if (cluster<2 || cluster>=F_CLUSTER_RESERVED) return F_ERR_EOF;
		_f_clustertopos(vi,cluster,&f->pos);

	}

	return F_ERR_EOF;
}

/****************************************************************************
 *
 * _f_getdecluster
 *
 * get a directory entry structure start cluster value
 *
 * INPUTS
 *
 * vi - volume pointer
 * de - directory entry
 *
 * RETURNS
 *
 * directory entry cluster value
 *
 ***************************************************************************/

unsigned long _f_getdecluster(const F_VOLUME *vi,F_DIRENTRY *de)
{
	unsigned long cluster;

	if (vi->mediatype==F_FAT32_MEDIA)
	{
		cluster=_f_get16bitl(de->clusterhi);
		cluster<<=16;
		cluster|=_f_get16bitl(de->clusterlo);
		return cluster;
	}

	return _f_get16bitl(de->clusterlo);
}

/****************************************************************************
 *
 * _f_setdecluster
 *
 * set a directory entry structure start cluster value
 *
 * INPUTS
 *
 * vi - volume pointer
 * de - directory entry
 * cluster - value of the start cluster
 *
 ***************************************************************************/

void _f_setdecluster(const F_VOLUME *vi,F_DIRENTRY *de,unsigned long cluster)
{
	_f_set16bitl(de->clusterlo,(unsigned short)(cluster&0xffff));

	if (vi->mediatype==F_FAT32_MEDIA)
	{
		_f_set16bitl(de->clusterhi,(unsigned short)((cluster>>16)&0xffff));
	}
	else
	{
		_f_set16bitl(de->clusterhi,(unsigned short)0);
	}
}

/****************************************************************************
 *
 * fn_getdrive
 *
 * Get current drive number
 *
 * INPUTS
 *
 * fm - multi structure pointer
 *
 * RETURNS
 *
 * with the current drive number (0-A, 1-B,...)
 *
 ***************************************************************************/

int fn_getdrive(F_MULTI *fm)
{
	F_SETLASTERROR_NORET(F_NO_ERROR);
#if USE_TASK_SEPARATED_CWD
	return fm->f_curdrive;
#else
	return f_filesystem.f_curdrive;
#endif
}

/****************************************************************************
 *
 * fn_chdrive
 *
 * Change current drive
 *
 * INPUTS
 *
 * fm - multi structure pointer
 * drivenum - new current drive number (0-A, 1-B, ...)
 *
 * RETURNS
 *
 * error code or zero if successful
 *
 ***************************************************************************/

int fn_chdrive(F_MULTI *fm,int drivenum)
{
	F_VOLUME *vi;

	if (drivenum < 0 || drivenum>=FN_MAXVOLUME) return F_SETLASTERROR(F_ERR_INVALIDDRIVE);

	vi=&f_filesystem.volumes[drivenum];

	if (vi->state==F_STATE_NONE) return F_SETLASTERROR(F_ERR_INVALIDDRIVE);

#if USE_TASK_SEPARATED_CWD
	fm->f_curdrive=drivenum;
#else
	f_filesystem.f_curdrive=drivenum;
#endif

	return F_SETLASTERROR(F_NO_ERROR);
}

/****************************************************************************
 *
 * _f_initentry
 *
 * init directory entry, this function is called if a new entry is coming
 *
 * INPUTS
 *
 * de - directory entry which needs to be initialized
 * name - fil ename  (8)
 * ext - file extension (3)
 *
 ***************************************************************************/

void _f_initentry(F_DIRENTRY *de,const char *name,const char *ext)
{
	unsigned short time,date;

	(void)_memset ((void *)de,0,sizeof(F_DIRENTRY)); /* reset all entries */

	(void)_memcpy (de->name,(void*)name,sizeof(de->name));
	(void)_memcpy (de->ext,(void*)ext,sizeof(de->ext));

	f_getcurrenttimedate(&time,&date);
	_f_set16bitl(de->cdate,date);  /* if there is realtime clock then creation date could be set from */
	_f_set16bitl(de->ctime,time);  /* if there is realtime clock then creation time could be set from */
}

/****************************************************************************
 *
 * _f_alloccluster
 *
 * allocate cluster from FAT
 *
 * INPUTS
 *
 * vi - volume pointer
 * pcluster - where to store the allocated cluster number
 *
 * RETURNS
 *
 * error code or zero if successful
 *
 ***************************************************************************/

int _f_alloccluster(F_VOLUME *vi,unsigned long *pcluster)
{
	unsigned long maxcluster=_f_getmaxcluster(vi);
	unsigned long cou;
	unsigned long cou2;
	unsigned long cluster=vi->lastalloccluster;
	unsigned long value;
	int ret;

	if (vi->mediatype==F_FAT32_MEDIA)
	{
		unsigned long sector=cluster;
		unsigned long sectorperfat=vi->firstfat.num;
	    sector/=((vi->bootrecord.bytes_per_sector)/4);
		cluster-=sector*((vi->bootrecord.bytes_per_sector)/4);

		for (cou=0; cou<sectorperfat; cou++)
		{
			unsigned char *cptr;

			if (sector>=sectorperfat) sector=0;
#ifdef FATBITFIELD_ENABLE
			if (!(vi->fatbitfield[sector >> 3]&(unsigned char)(1<<(sector&7))))
#endif
			{
				ret=_f_getfatsector(vi,sector);
				if (ret) return ret;

				cptr=vi->fat; /* assigned here, bacuse of caching mechanism */
				cptr+=(cluster<<2); /* translation to start */

				for (cou2=cluster; cou2<(unsigned long)((vi->bootrecord.bytes_per_sector)/4); cou2++, cptr+=4)
				{
					if (_f_get32bitl(cptr)==0)
					{
						cluster=sector*((vi->bootrecord.bytes_per_sector)/4)+cou2;

						if (cluster>=maxcluster+2) continue;
	
						vi->lastalloccluster=cluster+1; /* set next one */
						if (pcluster) *pcluster=cluster;
	
						if (vi->cspaceok)
						{
							vi->clspace.clfree--;
							vi->clspace.clused++;
						}
#ifdef FATBITFIELD_ENABLE
						if (cou2+1==(unsigned long)((vi->bootrecord.bytes_per_sector)/4))
						{
							cptr=vi->fat; /* check again if its all used */
							for (cou2=0; cou2<(unsigned long)((vi->bootrecord.bytes_per_sector)/4); cou2++, cptr+=4)
							{
								if (_f_get32bitl(cptr)==0)
								{
									vi->lastalloccluster-=(vi->bootrecord.bytes_per_sector)/4; /* set up back */
									vi->lastalloccluster+=cou2;			   /* set to pos */
									return F_NO_ERROR;
								}
							}

							vi->fatbitfield[sector >> 3]|=(unsigned char)(1<<(sector&7)); /* last allocated there is no free cluster*/
						}
#endif
	
						return F_NO_ERROR;
					}
				}

#ifdef FATBITFIELD_ENABLE
				vi->fatbitfield[sector >> 3]|=(unsigned char)(1<<(sector&7)); /* no free in this sector*/
#endif
			}

			cluster=0;
			sector++;
		}

		return F_ERR_NOMOREENTRY;
	}


	for (cou=0; cou<maxcluster; cou++)
	{

		/* 1st 2 sectors are reserved */
		if (cluster>=maxcluster+2) cluster=2;

		ret=_f_getclustervalue(vi,cluster,&value);
		if (ret) return ret;

		if (!value)
		{
			vi->lastalloccluster=cluster+1; /* set next one */
			if (pcluster) *pcluster=cluster;

			if (vi->cspaceok)
			{
				vi->clspace.clfree--;
				vi->clspace.clused++;
			}

			return F_NO_ERROR;
		}

		cluster++;
	}

	return F_ERR_NOMOREENTRY;
}

/****************************************************************************
 *
 * _f_dobadblock
 *
 * Bad block handler, if a given block is bad, this routine mark the cluster
 * as bad, and change the file pointer position values
 *
 * INPUTS
 *
 * vi - volume info
 * f - internal file pointer which contains bad block
 *
 * RETURNS
 *
 * error code or zero if successful
 *
 ***************************************************************************/

int _f_dobadblock(F_VOLUME *vi,FN_FILEINT *f)
{
	unsigned long currdif=f->pos.sectorend-f->pos.sector;	/* save current sector relative */
	F_POS posold;
	F_POS pos;
	unsigned long cluster=F_CLUSTER_BAD;
	unsigned long oldclustervalue;
	int badblockswapdone=0;
	int ret;

	ret=_f_getclustervalue(vi,f->pos.cluster,&oldclustervalue); /* get old value */
	if (ret) return ret;

	ret=_f_setclustervalue(vi,f->pos.cluster,F_CLUSTER_BAD); /* set signal as bad */
	if (ret) return ret;

	/* creating a loop for writing new cluster */
	while (!badblockswapdone)
	{
		badblockswapdone=1;	/* set as done */

		if (vi->cspaceok)	/* if space calculation optimalization is running */
		{
	 		vi->clspace.clused--; /* decreased used clusters */
	 		vi->clspace.clbad++;  /* increase bad clusters */
		}

		ret=_f_alloccluster(vi,&cluster);	/* get a new cluster */
		if (ret) return ret;

		ret=_f_setclustervalue(vi,cluster,oldclustervalue);	/* set old value back */
		if (ret) return ret;

		_f_clustertopos(vi,cluster,&pos);			   /* set new data position */
		_f_clustertopos(vi,f->pos.cluster,&posold); /* set old data position */

		/* copy all original cluster data contents to the new cluster */
		for (;pos.sector<pos.sectorend; pos.sector++,posold.sector++)
		{
			/* read original */
			ret=_f_readsector(vi,vi->sectorbuffer,posold.sector,1);
			if (ret) return ret; /* cannot read, bad block handling cannot be done !!! */

			/* write into new cluster */
			ret=_f_writesector(vi,vi->sectorbuffer,pos.sector,1);
			if (ret)
			{
				if (vi->state!=F_STATE_WORKING) return ret; /* not working */

				ret=_f_setclustervalue(vi,cluster,F_CLUSTER_BAD); /* set signal as bad also */
				if (ret) return ret;

				badblockswapdone=0;	/* reset back done */

				break;
			}
		}
	}


	/* recalc current sector according new cluster*/
	{
		unsigned long prevcluster;

		prevcluster=f->pos.prevcluster;			/* save previous cluster */
	 	_f_clustertopos(vi,cluster,&f->pos);	/* reset all variables tu new cluster */
		f->pos.prevcluster=prevcluster;			/* restore previous cluster */
	 	f->pos.sector=f->pos.sectorend-currdif; /* lets go back to current sector */
	}

	return F_NO_ERROR;
}

/****************************************************************************
 *
 * _f_checklocked
 *
 * check if a given file is locked or not
 *
 * INPUTS
 *
 * drvnum - drive number
 * pos - pos structure contains directory entry pos
 *
 * RETURNS
 *
 * zero - if not locked
 * F_ERR_LOCKED - if locked
 *
 ***************************************************************************/

int _f_checklocked(long drvnum,const F_POS *pos)
{
	int a;

	for (a=0; a<F_MAXFILES; a++)
	{
		FN_FILEINT *f=&f_filesystem.files[a];
		if (f->mode!=FN_FILE_CLOSE && f->mode!=FN_FILE_LOCKED)
		{
			if (drvnum==f->drivenum && f->dirpos.pos==pos->pos && f->dirpos.sector==pos->sector) return F_ERR_LOCKED;
		}
	}
	return F_NO_ERROR;
}

/****************************************************************************
 *
 * _f_checkappendlocked
 *
 * check if a given file is locked for appending
 *
 * INPUTS
 *
 * drvnum - drive number
 * pos - pos structure contains directory entry pos
 * ofile - original file
 *
 * RETURNS
 *
 * zero - if not locked
 * F_ERR_LOCKED - if locked
 *
 ***************************************************************************/

int _f_checkappendlocked(long drvnum,const F_POS *pos,FN_FILEINT *ofile)
{
	int a;

	/* check all opened files */
	for (a=0; a<F_MAXFILES; a++)
	{
		FN_FILEINT *f=&f_filesystem.files[a];

		if (f->mode!=FN_FILE_CLOSE && f->mode!=FN_FILE_LOCKED) /* if its not closed */
		{	/* same drive? same direntry? same dirsector? */
			if (drvnum==f->drivenum && f->dirpos.pos==pos->pos && f->dirpos.sector==pos->sector)
			{
				if (f->mode!=FN_FILE_RD) /* check if it is opened for read */
				{
					return F_ERR_LOCKED; /* it is locked */
				}
				else
				{
				   	ofile->state|=F_FILE_ST_SYNC; /* set sync */
				}
			}
		}
	}

	return F_NO_ERROR;
}

/****************************************************************************
 *
 * _f_syncfiles
 *
 * this function syncronizing file-s (appended and read files) filesizes
 *
 * INPUTS
 *
 * vi - volume info
 * fm - multi structure pointer
 * file - original file
 *
 ***************************************************************************/

void _f_syncfiles(F_VOLUME *vi,F_MULTI *fm,const FN_FILEINT *file)
{
	int a;

	if (file->mode==FN_FILE_AP || file->mode==FN_FILE_A)
	{
		/* check all opened files */
		for (a=0; a<F_MAXFILES; a++)
		{
			FN_FILEINT *f=&f_filesystem.files[a];

			if (f==file) continue; /* this is the same file */

			if (f->mode!=FN_FILE_CLOSE) /* if its not closed */
			{	/* same drive? same direntry? same dirsector? */
				if (file->drivenum==f->drivenum && file->dirpos.pos==f->dirpos.pos && f->dirpos.sector==file->dirpos.sector)
				{

					if (f->mode==FN_FILE_RD)
					{
						f->filesize=file->filesize; /* copy size to others */
						if (!f->startcluster)
						{
							if (file->startcluster)
							{
								f->startcluster=file->startcluster;
								_f_clustertopos(vi,f->startcluster,&f->pos);
								f->abspos=0; /* forcing seek to read 1st sector (abspos has to be in different sector */
								(void)_f_fseek(vi,fm,f,0,1);
							}
						}

					}
				}
			}
		}
	}
}

/****************************************************************************
 *
 * _f_syncfilescontent
 *
 * this function syncronizing file-s (appended and read files) content
 *
 * INPUTS
 *
 * file - original file
 * data - where the data is
 * start - start position in the updated area
 * wrsize - number of bytes in the updated area
 *
 ***************************************************************************/

void _f_syncfilescontent(FN_FILEINT *file,unsigned char *data, int start, int wrsize )
{
	int a;

	if (file->mode==FN_FILE_AP || file->mode==FN_FILE_A)
	{
		/* check all opened files */
		for (a=0; a<F_MAXFILES; a++)
		{
			FN_FILEINT *f=&f_filesystem.files[a];

			if (f==file) continue; /* this is the same file */

			if (f->mode!=FN_FILE_CLOSE) /* if its not closed */
			{	/* same drive? same direntry? same dirsector? */
				if (file->drivenum==f->drivenum && file->dirpos.pos==f->dirpos.pos && f->dirpos.sector==file->dirpos.sector)
				{
					if (f->mode==FN_FILE_RD)
					{
						if (f->abspos == file->abspos)
						{
							/* copy updated data into read file data area */
							(void)_memcpy(f->data+start,data+start,(unsigned int)wrsize);
						}
					}
				}
			}
		}
	}
}

/****************************************************************************
 *
 * fn_getfreespace
 *
 * get total/free/used/bad diskspace
 *
 * INPUTS
 *
 * fm - multi structure pointer
 * drivenum - which drive free space is requested (0-A, 1-B, 2-C)
 * pspace - pointer where to store the information
 *
 * RETURNS
 * error code
 *
 ***************************************************************************/

int fn_getfreespace(F_MULTI *fm,int drivenum, FN_SPACE *pspace)
{
	int ret;
	F_VOLUME *vi;
	unsigned long clustersize;
	unsigned long maxcluster;
	unsigned long a;

	ret=_f_getvolume(fm,drivenum,&vi);
	if (ret) return F_SETLASTERROR(ret);

	maxcluster=_f_getmaxcluster(vi);

	pspace->total=maxcluster;
	pspace->free=0;
	pspace->used=0;
	pspace->bad=0;

	if (!vi->cspaceok)
	{ /* init time calculation */
   		if (vi->mediatype==F_FAT32_MEDIA)
		{
	        unsigned long sector=0;
	   		unsigned long sectorperfat=vi->firstfat.num;
	        unsigned long cluster=0;
	        unsigned long cou;
	        unsigned long cou2;
	   		for (cou=0; cou<sectorperfat && cluster<maxcluster; cou++)
			{
#ifdef FATBITFIELD_ENABLE
	            char allused=1;
#endif
	   			unsigned long *lptr;
				ret=_f_getfatsector(vi,sector);
				if (ret) return F_SETLASTERROR(ret);

				lptr=(unsigned long*)(vi->fat); /* assigned here, bacuse of caching mechanism */

				if (cou==0) cou2=2;
				       else cou2=0;

				for (;cou2<(unsigned long)((vi->bootrecord.bytes_per_sector)/4) && cluster<maxcluster; cou2++,cluster++)
				{
					unsigned long value=_f_get32bitl(lptr+cou2) & 0x0fffffffUL;

   			   		if (!value)
					{
						++(pspace->free);
#ifdef FATBITFIELD_ENABLE
						allused=0;
#endif
					}
					else
					{
						if (value==F_CLUSTER_BAD) ++(pspace->bad);
						else ++(pspace->used);
					}
				}
#ifdef FATBITFIELD_ENABLE
				if (allused)
				{
					vi->fatbitfield[sector >> 3]|=(unsigned char)(1<<(sector&7)); /* no free cluster*/
				}
#endif
				sector++;
			}
		}
		else
		{
			for (a=0; a<maxcluster; a++)
			{
				unsigned long value;

				ret=_f_getclustervalue(vi,a+2,&value);
				if (ret) return F_SETLASTERROR(ret);

				if (!value) ++(pspace->free);
				else
				{
					if (value==F_CLUSTER_BAD) ++(pspace->bad);
					else ++(pspace->used);
				}
			}
		}

		vi->clspace.clfree=pspace->free;
		vi->clspace.clused=pspace->used;
		vi->clspace.clbad =pspace->bad ;
		vi->cspaceok=1;
	}
	else
	{ /* runtime calculeted */
		pspace->free=vi->clspace.clfree;
		pspace->used=vi->clspace.clused;
		pspace->bad=vi->clspace.clbad;
	}


	clustersize=vi->bootrecord.sector_per_cluster;
	clustersize*=(vi->bootrecord.bytes_per_sector);

	for (a=0;(clustersize&1)==0;a++) clustersize>>=1;
	pspace->total_high=(pspace->total)>>(32-a);
	pspace->total<<=a;
	pspace->free_high=(pspace->free)>>(32-a);
	pspace->free<<=a;
	pspace->used_high=(pspace->used)>>(32-a);
	pspace->used<<=a;
	pspace->bad_high=(pspace->bad)>>(32-a);
	pspace->bad<<=a;

	return F_SETLASTERROR(F_NO_ERROR);
}

/****************************************************************************
 *
 * _f_emptywritecache
 *
 * Write all the entries in the cache to the volume.
 * Write caching only works with fn_write. The concept is that if fn_write
 * is called with big size, the continous sector size writes are done
 * in one step, except if the cache is full.
 * In this way the intermediate filesystem buffer is not used, so we save
 * a lot of memcpy-s and continous sectors are written at the same time.
 *
 * INPUTS
 *
 * vi - volume info
 * f - internal filepointer
 *
 * RETURN
 *
 * errorcode or zero if successful
 *
 ***************************************************************************/

int _f_emptywritecache(F_VOLUME *vi, FN_FILEINT *f)
{
	int i,j;
	int ret;

	i=0;
	j=0;
	ret=0;

	while (i<f->WrDataCache.N)
	{
		for (j=i+1;j<f->WrDataCache.N && ((f->WrDataCache.pos+(j-1))->sector)+1==(f->WrDataCache.pos+j)->sector && (unsigned)(j-i)<256;j++)
		  ;
		ret=_f_writesector(vi,f->WrDataCache.ptr+i*(vi->bootrecord.bytes_per_sector),(f->WrDataCache.pos+i)->sector,j-i);
		if (ret) break;
		i=j;
	}

	if (ret)
	{
		F_POS act_pos=f->pos; /* save current position */

		for (;i<f->WrDataCache.N;i++)
		{
			int bad=0;

			(void)_memcpy(&(f->pos),f->WrDataCache.pos+i,sizeof(F_POS)); /* get data cache pos */

			for (;;) /* creating a forever loop because of bad block handling */
			{
				ret=_f_writesector(vi,f->WrDataCache.ptr+i*(vi->bootrecord.bytes_per_sector),f->pos.sector,1); /* try to write it */
				if (!ret) break;	/* if success then break the loop */

		 		if (vi->state!=F_STATE_WORKING) return ret; /* check if still working volume */

				/* doing bad block handling */
	 			ret=_f_dobadblock(vi,f);
	 			if (ret) return ret;

				/* set cluster chain into new cluster from the previous */
				if (!f->pos.prevcluster) /* if this is the 1st cluster in the chain */
				{
					f->startcluster=f->pos.cluster;
				}
				else
				{	/* set previous to point to current */
					ret=_f_setclustervalue(vi,f->pos.prevcluster,f->pos.cluster);
					if (ret) return ret;
				}
				bad=1;
			}

			if (bad)
			{
				F_POS *s=f->WrDataCache.pos+i;

				/* check if actual position was in this bad cluster */
				if (act_pos.cluster==s->cluster)
				{	/* update information there */
					act_pos.cluster=f->pos.cluster;
					act_pos.sector=f->pos.sectorbegin+(act_pos.sector-act_pos.sectorbegin);
					act_pos.sectorbegin=f->pos.sectorbegin;
					act_pos.sectorend=f->pos.sectorend;
				}

				/* check prev clusters also */
				if (act_pos.prevcluster==s->cluster)
				{
					act_pos.prevcluster=f->pos.cluster;
				}

				for (j=f->WrDataCache.N-1;j>i;j--)
				{
					F_POS *d=f->WrDataCache.pos+j;

					/* check whether other cluster used this bad cluster */
					if (d->cluster==s->cluster)
					{	/* update information there */
						d->cluster=f->pos.cluster;
						d->sector=f->pos.sectorbegin+(d->sector-d->sectorbegin);
						d->sectorbegin=f->pos.sectorbegin;
						d->sectorend=f->pos.sectorend;
					}

					/* check prev clusters also */
					if (d->prevcluster==s->cluster)
					{
						d->prevcluster=f->pos.cluster;
					}
				}

			}
		}

		f->pos=act_pos;	 /* restore current position */
	}

	f->WrDataCache.N=0;

	return F_NO_ERROR;
}

/****************************************************************************
 *
 * _f_allocnextcluster
 *
 * Allocates next cluster for a file if needed
 *
 * INPUT
 *
 * vi - volume pointer
 * f - FN_FILEINT structure which file position needed to be modified
 *
 * RETURN
 *
 * 0 - on success / other if error
 *
 ***************************************************************************/

int _f_allocnextcluster(F_VOLUME *vi,FN_FILEINT *f)
{
	int ret;
	if (f->pos.sector>=f->pos.sectorend)	/* check if sector is still in current cluster */
	{										/* if not */
		if (!f->startcluster)				/* check if cluster chain is existing */
		{
			ret=_f_alloccluster(vi,&f->startcluster);	/* if not, then allocate the 1st cluster */
			if (ret) return ret;

			_f_clustertopos(vi,f->startcluster,&f->pos);/* calculate new sector position */

			ret=_f_setclustervalue(vi,f->startcluster,F_CLUSTER_LAST); /* set cluster value as last cluster */
			if (ret) return ret;
		}
		else
		{
			unsigned long value;

			ret=_f_getclustervalue(vi,f->pos.cluster,&value); /* get the current cluster value */
			if (ret) return ret;

			if (value<F_CLUSTER_RESERVED) /* check cluster value */
			{	/* we are in the middle chain */
				_f_clustertopos(vi,value,&f->pos); /* go to next cluster */
			}
			else
			{	/* we are at the end of the chain */
				unsigned long nextcluster;

				ret=_f_alloccluster(vi,&nextcluster);	/* alloc new cluster */
				if (ret) return ret;

				ret=_f_setclustervalue(vi,nextcluster,F_CLUSTER_LAST); /* set as last cluster */
				if (ret) return ret;

				ret=_f_setclustervalue(vi,f->pos.cluster,nextcluster); /* set cluster chain to point next */
				if (ret) return ret;

				_f_clustertopos(vi,nextcluster,&f->pos);	/* calculate new sector position */
			}
		}
	}

	return F_NO_ERROR;
}

/****************************************************************************
 *
 * _f_emptywritebuffer
 *
 * empty write buffer if it contains unwritten data
 *
 * INPUTS
 *
 * vi - volume pointer
 * f - internal file pointer
 *
 * RETURNS
 *
 * error code or zero if successful
 *
 ***************************************************************************/

int _f_emptywritebuffer(F_VOLUME *vi,FN_FILEINT *f,char *ptr)
{
	int ret;

	if (!f->modified) return F_NO_ERROR;	/* if not modified then nothing to do */

	ret=_f_allocnextcluster(vi,f);
	if (ret) return ret;

	if (ptr)
	{
		if (f->WrDataCache.N==WR_DATACACHE_SIZE)
		{
			ret=_f_emptywritecache(vi,f);
			if (ret) return ret;
		}

		if (f->WrDataCache.N==0) f->WrDataCache.ptr=ptr;

		(void)_memcpy(f->WrDataCache.pos+(f->WrDataCache.N++),&(f->pos),sizeof(F_POS));
	}
	else
	{
		for (;f->datawritten==0;) /* creating a forever loop because of bad block handling */
		{
			ret=_f_writesector(vi,f->data,f->pos.sector,1); /* try to write it */
			if (!ret) break;	/* if success then break the loop */

	 		if (vi->state!=F_STATE_WORKING) return ret; /* check if still working volume */

			/* doing bad block handling */
	 		ret=_f_dobadblock(vi,f);
	 		if (ret) return ret;

			/* set cluster chain into new cluster from the previous */
			if (!f->pos.prevcluster) /* if this is the 1st cluster in the chain */
			{
				f->startcluster=f->pos.cluster;
			}
			else
			{	/* set previous to point to current */
				ret=_f_setclustervalue(vi,f->pos.prevcluster,f->pos.cluster);
				if (ret) return ret;
			}
		}
	}

	f->modified=0; /* reset modified bit for write buffer */

	return F_NO_ERROR;
}

/****************************************************************************
 *
 * _f_writezeros
 *
 * subfunction for seek and truncate to fill zeroes at the end
 *
 * INPUTS
 *
 * vi - volume info
 * fm - multiuser structure
 * f - FN_FILEINT structure which file position needed to be modified
 * num - number of zeroes to be written
 *
 * RETURNS
 *
 * error code or zero if successful
 *
 ***************************************************************************/

int _f_writezeros(F_VOLUME *vi,F_MULTI *fm,FN_FILEINT *f,unsigned long num)
{
	(void)_memset(vi->sectorbuffer,0,(vi->bootrecord.bytes_per_sector)); /* set zeroes to truncate */

	while (num)
	{
		unsigned long size=num;
		long retsize;

		if (size>(vi->bootrecord.bytes_per_sector)) 
		{
			size=(vi->bootrecord.bytes_per_sector);
		}

		retsize=fn_write(fm,vi->sectorbuffer,1,(long)size,(FN_FILE *)(f->file.reference));
		if ((long)size!=retsize)
		{
#ifdef USE_MALLOC
			if (f->WrDataCache.pos) 
			{
				__free(f->WrDataCache.pos);
				f->WrDataCache.pos=NULL;
			}
#endif
			f->mode|=FN_FILE_ABORT_FLAG;
			return F_ERR_WRITE;
		}
		num-=size;
	}

	return F_NO_ERROR;
}

/****************************************************************************
 *
 * _f_fseek
 *
 * subfunction for f_seek it moves position into given offset and read
 * the current sector
 *
 * INPUTS
 *
 * vi - volume pointer
 * fm - multiuser structure
 * f - FN_FILEINT structure which file position needed to be modified
 * offset - position from start
 * forceread - forcing sector read  
 *
 * RETURNS
 *
 * error code or zero if successful
 *
 ***************************************************************************/

int _f_fseek(F_VOLUME *vi,F_MULTI *fm,FN_FILEINT *f,unsigned long offset, char forceread)
{
	unsigned long remainingbytes;
	unsigned long cluster;
	unsigned long clustersize;
	int ret;

	/* check weather we seeking beyond eof */
	if (offset<=f->filesize)
	{
		remainingbytes=0;
	}
	else
	{	/* yes we are seeking beyond eof */
		if (f->mode!=FN_FILE_RDP && f->mode!=FN_FILE_WR && f->mode!=FN_FILE_WRP && f->mode!=FN_FILE_A && f->mode!=FN_FILE_AP) return F_ERR_NOTOPEN;
		remainingbytes=offset-f->filesize;
		offset=f->filesize;
	}

	if (!forceread)
	{
		/* check wheter we are seeking in the current sector */
		if (offset>=f->abspos && offset<f->abspos+(vi->bootrecord.bytes_per_sector) && (!remainingbytes))
		{
			f->relpos=offset-f->abspos;
			return F_NO_ERROR;
		}
	}

	/* check if file buffer was modified last time */
	if ( f->modified )
	{
		ret=_f_emptywritebuffer(vi,f,NULL);
		if (ret)
		{
#ifdef USE_MALLOC
			if (f->WrDataCache.pos) 
			{
				__free(f->WrDataCache.pos);
				f->WrDataCache.pos=NULL;
			}
#endif
		 	f->mode|=FN_FILE_ABORT_FLAG; /* cant accessed any more */
			return ret;
		}

		ret=_f_writefatsector(vi);
		if (ret)
		{
#ifdef USE_MALLOC
			if (f->WrDataCache.pos) 
			{
				__free(f->WrDataCache.pos);
				f->WrDataCache.pos=NULL;
			}
#endif
			f->mode|=FN_FILE_ABORT_FLAG;
			return ret;
		}

#if F_MAXSEEKPOS
		_fn_updateseekpos(f);
#endif
	}

	if (!f->filesize)
	{
		/* seeking in no filesize file */
		if (remainingbytes)
		{
			return	_f_writezeros(vi,fm,f,remainingbytes);
		}

		return F_NO_ERROR;
	}

	clustersize=vi->bootrecord.sector_per_cluster;
	clustersize*=(vi->bootrecord.bytes_per_sector);

	{
		unsigned long seek_cluster = offset / clustersize;
		unsigned long curr_cluster = f->abspos / clustersize;

/*  check offset in the seekpos table compared by f->abspos */
#if F_MAXSEEKPOS
		char set=1;

		if (seek_cluster >= curr_cluster)
		{
			f->abspos = curr_cluster * clustersize;
			set=0;
		}
		else
		{
			f->abspos = 0;
			f->pos.prevcluster = 0;
			f->pos.cluster = f->startcluster;
		}

		{
			long pos=(long)(offset >> f->seekshift);

			pos--;
			if (pos>=F_MAXSEEKPOS) pos=F_MAXSEEKPOS-1;

			for (;pos>=0; pos--)
			{
				if (f->seekpos[pos]!=-1)
				{
					/* check whether new position is closer */
					if (f->abspos <(unsigned long)((pos+1)<<f->seekshift))
					{
						f->abspos=(unsigned long)((pos+1)<<f->seekshift);
						offset-=(pos+1)<<f->seekshift;
						f->pos.cluster=(unsigned long)f->seekpos[pos];
						f->pos.prevcluster=(unsigned long)f->seekprev[pos];
						set=1;
					}
					else
					{
						offset -= curr_cluster * clustersize;
						set=1;
					}
					break;
				}
			}

			/* check whether offset is calculated already */
			if (!set)
			{
			 	offset -= curr_cluster * clustersize;
			}
		}
#else
		if (seek_cluster >= curr_cluster)
		{
			/* Seek is in the current/following clusters */
			f->abspos = curr_cluster * clustersize;
			offset -= curr_cluster * clustersize;
		}
		else
		{
			f->abspos = 0;
			f->pos.prevcluster = 0;
			f->pos.cluster = f->startcluster;
		}
#endif
	 	f->relpos = 0;
	}

	/* calc cluster */
	while (offset>=clustersize)
	{
		ret=_f_getclustervalue(vi,f->pos.cluster,&cluster);
		if (ret)
		{
#ifdef USE_MALLOC
			if (f->WrDataCache.pos) 
			{
				__free(f->WrDataCache.pos);
				f->WrDataCache.pos=NULL;
			}
#endif
			f->mode|=FN_FILE_ABORT_FLAG;
			return ret;
		}

		offset-=clustersize;
		f->abspos+=clustersize;

		if (cluster>=F_CLUSTER_RESERVED)
		{
			if (!offset)
			{
				unsigned long prev=f->pos.prevcluster;

				_f_clustertopos(vi,f->pos.cluster,&f->pos);
				f->pos.prevcluster=prev;

				f->pos.sector=f->pos.sectorend;

				if (remainingbytes)
				{
					return	_f_writezeros(vi,fm,f,remainingbytes);
				}

				return F_NO_ERROR; /* but eof! */
			}

#ifdef USE_MALLOC
			if (f->WrDataCache.pos) 
			{
				__free(f->WrDataCache.pos);
				f->WrDataCache.pos=NULL;
			}
#endif
			f->mode|=FN_FILE_ABORT_FLAG;
			return F_ERR_INVALIDSECTOR;
		}

		f->pos.prevcluster=f->pos.cluster;
		f->pos.cluster=cluster;

#if F_MAXSEEKPOS
		_fn_updateseekpos(f);
#endif
	}

	{
		unsigned long prev=f->pos.prevcluster;
		_f_clustertopos(vi,f->pos.cluster,&f->pos);
		f->pos.prevcluster=prev;
	}

	while (offset>=(vi->bootrecord.bytes_per_sector))
	{
		f->pos.sector++;

		offset-=(vi->bootrecord.bytes_per_sector);
		f->abspos+=(vi->bootrecord.bytes_per_sector);
	}

	f->relpos=offset;

	ret=_f_getcurrsector(vi,f,NULL,0);
	if (ret) return ret;

#if F_MAXSEEKPOS
	_fn_updateseekpos(f);
#endif

	if (remainingbytes)
	{
		return	_f_writezeros(vi,fm,f,remainingbytes);
	}

	return F_NO_ERROR;
}

/****************************************************************************
 *
 * _f_removechain
 *
 * remove cluster chain from fat
 *
 * INPUTS
 *
 * vi - volume pointer
 * cluster - first cluster in the cluster chain
 *
 * RETURNS
 *
 * error code or zero if successful
 *
 ***************************************************************************/

int _f_removechain(F_VOLUME *vi,unsigned long cluster)
{
	if (cluster<vi->lastalloccluster)
	{ /* this could be the begining of alloc */
		vi->lastalloccluster=cluster;
	}

	while (cluster<F_CLUSTER_RESERVED && cluster>=2)
	{
		unsigned long nextcluster;

		int ret=_f_getclustervalue(vi,cluster,&nextcluster);
		if (ret) return ret;

		ret=_f_setclustervalue(vi,cluster,F_CLUSTER_FREE);
		if (ret) return ret;

		if (vi->cspaceok)
		{
		 	vi->clspace.clused--;
		 	vi->clspace.clfree++;
		}

#ifdef FATBITFIELD_ENABLE
		if (vi->mediatype==F_FAT32_MEDIA)
		{
			unsigned long sector=cluster/((vi->bootrecord.bytes_per_sector)/4);
			vi->fatbitfield[sector >> 3]&=~(unsigned char)(1<<(sector&7));
		}
#endif
		cluster=nextcluster;
	}

	return _f_writefatsector(vi);
}

/****************************************************************************
 *
 * _f_removechainandcontent
 *
 * remove cluster chain from fat and also reset cluster's sector content
 *
 * INPUTS
 *
 * vi - volume pointer
 * cluster - first cluster in the cluster chain
 *
 * RETURNS
 *
 * error code or zero if successful
 *
 ***************************************************************************/

#if F_DELETE_CONTENT
int _f_removechainandcontent(F_VOLUME *vi,unsigned long cluster)
{
	int ret;

	if (cluster<vi->lastalloccluster)
	{ /* this could be the begining of alloc */
		vi->lastalloccluster=cluster;
	}

	/* set 0xff into sectorbuffer */
	(void)_memset(vi->sectorbuffer,0xff,(vi->bootrecord.bytes_per_sector)); 

	while (cluster<F_CLUSTER_RESERVED && cluster>=2)
	{
		unsigned long nextcluster;

		ret=_f_getclustervalue(vi,cluster,&nextcluster);
		if (ret) return ret;

		/* erase content of the clusters */
		{
			unsigned long sector;
			int sectorpercluster;
			F_POS pos;
			_f_clustertopos(vi,cluster,&pos);

			/* do sector erase accelerated if possible, if not then do it manually */
			if (_f_ioctl_multiplesectorerase(vi,pos.sectorbegin,vi->bootrecord.sector_per_cluster,vi->sectorbuffer))
			{
				sector=pos.sectorbegin;
				sectorpercluster=vi->bootrecord.sector_per_cluster;

				while (sectorpercluster--)
				{
					ret=_f_writesector(vi,vi->sectorbuffer,sector,1);

					if (ret)
					{
						return ret;
					}

					sector++;
				}
			}
		}

		ret=_f_setclustervalue(vi,cluster,F_CLUSTER_FREE);
		if (ret) return ret;

		if (vi->cspaceok)
		{
		 	vi->clspace.clused--;
		 	vi->clspace.clfree++;
		}

#ifdef FATBITFIELD_ENABLE
		if (vi->mediatype==F_FAT32_MEDIA)
		{
			unsigned long sector=cluster/((vi->bootrecord.bytes_per_sector)/4);
			vi->fatbitfield[sector >> 3]&=~(unsigned char)(1<<(sector&7));
		}
#endif
		cluster=nextcluster;
	}

	ret=_f_ioctl_endofdelete(vi);
	if (ret) return ret;

	return _f_writefatsector(vi);
}
#endif

/****************************************************************************
 *
 * _f_updatefileentry
 *
 * Updated a file directory entry or removes the entry
 * and the fat chain belonging to it.
 *
 * INPUTS
 *
 * vi - volume info
 * f - internal file handler
 * remove - flag if remove is necessary
 *
 * RETURNS
 *
 * error code or zero if successful
 *
 ***************************************************************************/

int _f_updatefileentry (F_VOLUME *vi, FN_FILEINT *f, int remove)
{
	F_DIRENTRY *de;
	unsigned short time,date;
	int ret;

	if (remove)
	{
		f->mode |= FN_FILE_ABORT_FLAG; /* file is aborted for further access */
	}

	de=(F_DIRENTRY*)(vi->direntry+sizeof(F_DIRENTRY)*f->dirpos.pos);

	ret=_f_getdirsector(vi,f->dirpos.sector);
	if (ret) return ret;

	_f_setdecluster(vi,de,f->startcluster);
	_f_set32bitl(de->filesize,f->filesize);

	f_getcurrenttimedate(&time,&date);
	_f_set16bitl(de->cdate,date);  /* if there is realtime clock then creation date could be set from */
	_f_set16bitl(de->ctime,time);  /* if there is realtime clock then creation time could be set from */
	_f_set16bitl(de->lastaccessdate,date);  /* if there is realtime clock then lastaccess date could be set from */

#if F_FILE_CHANGED_EVENT
	if (f_filechangedevent)
	{
		ST_FILE_CHANGED fc;

		fc.action = FACTION_MODIFIED;

		fc.flags = FFLAGS_FILE_NAME | FFLAGS_LAST_WRITE | FFLAGS_SIZE;

		fc.attr=de->attr;
		fc.ctime=_f_get16bitl(de->ctime);
		fc.cdate=_f_get16bitl(de->cdate);
		fc.filesize=_f_get32bitl(de->filesize);

		_memcpy(fc.filename,f->filename,sizeof(fc.filename));

		f_filechangedevent(&fc);
	}
#endif

	return _f_writedirsector(vi);
}

/****************************************************************************
 *
 * _f_checkreadlocked
 *
 * check if a given file is locked or not for reading
 *
 * INPUTS
 *
 * vi - volume info
 * drvnum - drive number
 * pos - pos structure contains directory entry pos
 * fapp - output filepointer if it finds file which appends
 *
 * RETURNS
 *
 * zero - if not locked
 * F_ERR_LOCKED - if locked
 *
 ***************************************************************************/

int _f_checkreadlocked(F_VOLUME *vi,long drvnum,F_POS *pos,FN_FILEINT **fapp)
{
	int a;

	*fapp=0; /* reset it */

	/* check all opened files */
	for (a=0; a<F_MAXFILES; a++)
	{
		FN_FILEINT *f=&f_filesystem.files[a];

		if (f->mode!=FN_FILE_CLOSE && f->mode!=FN_FILE_LOCKED) /* if its not closed */
		{	/* same drive? same direntry? same dirsector? */
			if (drvnum==f->drivenum && f->dirpos.pos==pos->pos && f->dirpos.sector==pos->sector)
			{
				if (f->mode!=FN_FILE_RD) /* check if it is opened for read */
				{
					if (f->mode==FN_FILE_A || f->mode==FN_FILE_AP)
					{
						if (!(f->state&F_FILE_ST_SYNC)) /* check if already sync */
						{
					 		f->state|=F_FILE_ST_SYNC; /* set sync */

							if (_f_emptywritebuffer(vi,f,NULL))
							{
								(void)_f_updatefileentry(vi,f,1);
#ifdef USE_MALLOC
								if (f->WrDataCache.pos) 
								{
									__free(f->WrDataCache.pos);
									f->WrDataCache.pos=NULL;
								}
#endif
								f->mode|=FN_FILE_ABORT_FLAG;
								return 0;
							}
						}

						*fapp=f; /* store current file */
						continue; /* append and read is allowed */
					}

					return F_ERR_LOCKED; /* it is locked */
				}
			}
		}
	}

	return F_NO_ERROR;
}

/****************************************************************************
 *
 * _f_check_handle
 *
 * internal function it checks if a file handler is valid and converts it
 * into internal file handler
 *
 * INPUTS
 *
 * filehandle - which filehandle needs to be checked
 *
 * RETURNS
 *
 * 0 - if filehandle is not correct
 * FN_FILEINT structure pointer if successfully
 *
 ***************************************************************************/

FN_FILEINT *_f_check_handle(FN_FILE *filehandle)
{
	FN_FILEINT *f;
	unsigned long a=((1<<F_MAXFILES_SHIFT)-1);
	unsigned long fpos=(unsigned long)((unsigned long)filehandle & a);

	if (fpos>=F_MAXFILES) 
	{
		/* out of range */
		return 0; 
	}

	f=&f_filesystem.files[fpos];

	if (filehandle!=f->file.reference) 
	{
		/* drive build num differs */
		return 0;
	}

	return f;
}

/****************************************************************************
 *
 * fn_close
 *
 * close a previously opened file
 *
 * INPUTS
 *
 * fm - multi structure pointer
 * filehandle - which file needs to be closed
 *
 * RETURNS
 *
 * error code or zero if successful
 *
 ***************************************************************************/

int fn_close(F_MULTI *fm,FN_FILE *filehandle)
{
	FN_FILEINT *f=_f_check_handle(filehandle);
	int rc=F_ERR_NOTOPEN;
	F_VOLUME *vi;
	int ret,a;

	if (!f) return F_SETLASTERROR(rc);

	ret=_f_getvolume(fm,f->drivenum,&vi);
	if (ret) return F_SETLASTERROR(ret);

	/* remove connection with syncfiles */
	if (f->state & F_FILE_ST_SYNC)
	{
		for (a=0; a<F_MAXFILES; a++)
		{
			FN_FILEINT *file=&f_filesystem.files[a];
			if (file->syncfile==f)
			{
				file->syncfile=0; /* set disconnected */
			}
		}
	}

	/* check if it was aborted 1st */
	if (f->mode & FN_FILE_ABORT_FLAG)
	{
		rc=F_NO_ERROR;
	}
	else
	{
		switch (f->mode)
		{
			case FN_FILE_CLOSE:
				return F_SETLASTERROR(F_ERR_NOTOPEN);		   /* it was not open */

			case FN_FILE_RD:
				rc=F_NO_ERROR;
				break;

			case FN_FILE_RDP:
			case FN_FILE_WR:
			case FN_FILE_A:
			case FN_FILE_AP:
			case FN_FILE_WRP:

				if (_f_emptywritebuffer(vi,f,NULL))
				{
					(void)_f_updatefileentry(vi,f,1);
					rc=F_ERR_WRITE;
					break;
				}

				if (_f_writefatsector(vi))
				{
					(void)_f_updatefileentry(vi,f,1);
					rc=F_ERR_WRITE;
					break;
				}
#ifdef FATCACHE_ENABLE
				if (_f_fatcache_flush(vi))
				{
					rc=F_ERR_WRITE;
					break;
				}
#endif
				_f_syncfiles(vi,fm,f); /* f->mode must be correct */

				rc=_f_updatefileentry(vi,f,0);
				break;
		}
	}

#ifdef USE_MALLOC
	if (f->WrDataCache.pos) 
	{
		__free(f->WrDataCache.pos);
		f->WrDataCache.pos=NULL;
	}
#endif

	f->mode=FN_FILE_CLOSE;

  	return F_SETLASTERROR(rc);
}

/****************************************************************************
 *
 * fn_flush
 *
 * flushing current content a file into physical.
 *
 * INPUTS
 *
 * fm - multi structure pointer
 * filehandle - which file needs to be flushed
 *
 * RETURNS
 *
 * 0 - if successfully
 * other - if any error
 *
 ***************************************************************************/

int fn_flush(F_MULTI *fm,FN_FILE *filehandle)
{
	F_VOLUME *vi;
	FN_FILEINT *f=_f_check_handle(filehandle);
	int ret;

	if (!f) return F_SETLASTERROR(F_ERR_NOTOPEN);

	ret=_f_getvolume(fm,f->drivenum,&vi);
	if (ret) return F_SETLASTERROR(ret);

	switch (f->mode)
	{
	case FN_FILE_RDP:
	case FN_FILE_WR:
	case FN_FILE_A:
	case FN_FILE_AP:
	case FN_FILE_WRP:

				if (_f_emptywritebuffer(vi,f,NULL))
				{
					(void)_f_updatefileentry(vi,f,1);
					return F_SETLASTERROR(F_ERR_WRITE);
				}

				if (_f_writefatsector(vi))
				{
					(void)_f_updatefileentry(vi,f,1);
					return F_SETLASTERROR(F_ERR_WRITE);
				}
#ifdef FATCACHE_ENABLE
				if (_f_fatcache_flush(vi))
				{
					return F_SETLASTERROR(F_ERR_WRITE);
				}
#endif
				_f_syncfiles(vi,fm,f);

				ret=_f_updatefileentry(vi,f,0);
				return F_SETLASTERROR(ret);
	}

	return F_SETLASTERROR(F_ERR_NOTOPEN);		   /* it was not open */
}

/****************************************************************************
 *
 * fn_read
 *
 * read data from file
 *
 * INPUTS
 *
 * fm - multi structure pointer
 * buf - where the store data
 * size - size of items to be read
 * size_st - number of items need to be read
 * filehandle - file where to read from
 *
 * RETURNS
 *
 * with the number of read items
 *
 ***************************************************************************/

#if F_SUPPORT_TI64K
long fn_read_sub(F_MULTI *fm,void *buf,long size,long size_st,FN_FILE *filehandle)
#else
long fn_read(F_MULTI *fm,void *buf,long size,long size_st,FN_FILE *filehandle)
#endif
{
	FN_FILEINT *f=_f_check_handle(filehandle);
	char *buffer=(char*)buf;
	long retsize=0;
	long rs=size;
	int direct=0;
	unsigned int N=0;
	F_VOLUME *vi;
	int ret;

	/* check if file is open */
	if (!f)
	{
		F_SETLASTERROR_NORET(F_ERR_NOTOPEN);
		return 0;
	}

	/* check file mode validity only allowed for read r,r+,w+,a+ */
	if (f->mode!=FN_FILE_RD && f->mode!=FN_FILE_RDP && f->mode!=FN_FILE_WRP && f->mode!=FN_FILE_AP)
	{
		F_SETLASTERROR_NORET(F_ERR_NOTOPEN);
		return 0;
	}

	/* getting volume information */
	ret=_f_getvolume(fm,f->drivenum,&vi);
	if (ret)
	{
		F_SETLASTERROR_NORET(ret);
		return 0; /* cant read any */
	}

	/* calculate whole read size*/
	size*=size_st;

	/* size can be posititive number only, more than 2GB read is not allowed */
	if (size<0)
	{
	 	F_SETLASTERROR_NORET(F_ERR_UNKNOWN);
		return 0;
	}

	/* check if size is zero */
	if (size==0)
	{
	 	F_SETLASTERROR_NORET(F_NO_ERROR);
		return 0;
	}

	/* check whether read len longer than the file */
	if (size+f->relpos+f->abspos >= f->filesize)
	{	
		size=(long)((f->filesize)-(f->relpos)-(f->abspos));  /* calculate new size */

		if (size<=0)
		{
		 	F_SETLASTERROR_NORET(F_NO_ERROR);
			return 0;
		}
	}

	while (size)
	{
		unsigned long rdsize=(unsigned long)size;

		/* check if we reach the end of the file buffer */
		if (f->relpos==(vi->bootrecord.bytes_per_sector))
		{
			/* file buffer was modified before (e.g. fn_write)*/
			if ( f->modified )
			{
				/* empty write buffer automatically goes to next sector */
				ret=_f_emptywritebuffer(vi,f,NULL);
				if (ret)
				{
#ifdef USE_MALLOC
					if (f->WrDataCache.pos) 
					{
						__free(f->WrDataCache.pos);
						f->WrDataCache.pos=NULL;
					}
#endif
					f->mode|=FN_FILE_ABORT_FLAG; /* no more read allowed */
					F_SETLASTERROR_NORET(ret);
					return retsize/rs;
				}

				ret=_f_writefatsector(vi);
				if (ret)
				{
#ifdef USE_MALLOC
					if (f->WrDataCache.pos) 
					{
						__free(f->WrDataCache.pos);
						f->WrDataCache.pos=NULL;
					}
#endif
					f->mode|=FN_FILE_ABORT_FLAG;
					F_SETLASTERROR_NORET(ret);
					return retsize/rs;
				}
#if F_MAXSEEKPOS
				_fn_updateseekpos(f);
#endif
			}


			/* calculate new buffer positions */
			f->abspos+=f->relpos;
			f->relpos=0;

			/* goto next sector */
			f->pos.sector++;	

			/* if size is bigger than a sector */
			if (size>(vi->bootrecord.bytes_per_sector))
			{
				direct=1;
				/* calculate number of sector requested */
				N=(unsigned int)(size/(vi->bootrecord.bytes_per_sector));

				/* get the number of sectors direct into the buffer */
				ret=_f_getcurrsector(vi,f,buffer,&N);

				/* N's value is re-set in _f_getcurrsector according how many sector is read */
				if (N)
				{
					/* update last sector into file buffer if this was the last read sector */
					if (size==(long)N*(vi->bootrecord.bytes_per_sector))
					{
						(void)_memcpy(f->data,buffer+(N-1)*(vi->bootrecord.bytes_per_sector),(vi->bootrecord.bytes_per_sector));
					}

					/* update size, buffer, and return size */
					--N;
					N*=(vi->bootrecord.bytes_per_sector);
					buffer+=N;
					size-=N;
					retsize+=N;
					/* f->abspos+=N;*/ /* it is updated in getcurrsector in this case */
				}
			}
			else
			{
				direct=0;
				/* get content from volume data into file data buffer */
				ret=_f_getcurrsector(vi,f,NULL,0);
			}

#if F_MAXSEEKPOS
			_fn_updateseekpos(f);
#endif
			/* signal no error if EOF is detected but there is no file size */
			if (ret==F_ERR_EOF && (!size))
			{
				F_SETLASTERROR_NORET(F_NO_ERROR);
				return retsize/rs;
			}

			/* signal error if any error */
			if (ret)
			{
#ifdef USE_MALLOC
				if (f->WrDataCache.pos) 
				{
					__free(f->WrDataCache.pos);
					f->WrDataCache.pos=NULL;
				}
#endif
				f->mode|=FN_FILE_ABORT_FLAG; /* no more read allowed */
				F_SETLASTERROR_NORET(ret);
				return retsize/rs;
			}
		}

		/* set rdsize lessthan or equal as (vi->bootrecord.bytes_per_sector) */
		if (rdsize>=(vi->bootrecord.bytes_per_sector)-f->relpos)
		{
			rdsize=(vi->bootrecord.bytes_per_sector)-f->relpos;
		}

		/* if there wasn't direct copy then copy data */
		if (!direct)
		{
			unsigned char *ptr=f->data;

			/* check if this file has a syncronization file (a,a+ paralell write)*/
			if (f->syncfile)
			{
				char mode=f->syncfile->mode;

				/* only this a,a+ is allowed to be synced */
				if ( mode == FN_FILE_AP || mode == FN_FILE_A)
				{
					/* check range */
					if (f->abspos == f->syncfile->abspos)
					{
						ptr=f->syncfile->data; /* set source to syncfile buffer */
					}
				}
				else
				{
					/* other modes so disconnect */
					f->syncfile=0; 
				}
			}

			(void)_memcpy((void *)buffer,(void *)(ptr+f->relpos),(unsigned int)rdsize);
		}

		/* update buffer, position in filebuffer, size and return size */
		buffer+=rdsize;
		f->relpos+=rdsize;
		size-=rdsize;
		retsize+=rdsize;
	}

	F_SETLASTERROR_NORET(F_NO_ERROR);
	return retsize/rs;
}

/****************************************************************************
 *
 * fn_read
 *
 * read data from file, this is a TI extension   for handling 64K boundary
 * problem
 *
 * INPUTS
 *
 * fm - multi structure pointer
 * buf - where the store data
 * size - size of items to be read
 * size_st - number of items need to be read
 * filehandle - file where to read from
 *
 * RETURNS
 *
 * with the number of read items
 *
 ***************************************************************************/

#if F_SUPPORT_TI64K
long fn_read(F_MULTI *fm,void *buf,long size,long size_st,FN_FILE *filehandle)
{
	unsigned long data=(unsigned long)buf;
	long storedsize=0;

	long allsize=(long)(size * size_st);

	/* check if size is zero */
	if (allsize==0)
	{
	   	F_SETLASTERROR_NORET(F_NO_ERROR);
		return 0; /* size is zero */
	}

	/* check if size is negative more than 2 GB is not allowed */
	if (allsize<0)
	{
		F_SETLASTERROR_NORET(F_ERR_UNKNOWN);
		return 0; /* size is negative */
	}

	while (allsize)
	{
		long rsize;
		long dsize=(long)(0x10000 - (data & 0xffff));
		
		if (dsize > allsize)
		{
			dsize=allsize;
		}

		rsize=fn_read_sub(fm,(void *)data,1,dsize,filehandle);
		storedsize+=rsize;

		if (rsize!=dsize)
		{
			/* calculate all stored size divided by item size */
			return storedsize/size;
		}

		data+=dsize;
		allsize-=dsize;
	}

	F_SETLASTERROR_NORET(F_NO_ERROR);
	return size_st;
}
#endif

/****************************************************************************
 *
 * _f_abortwrite
 *
 * Aborts write, puts the file to closed state, empties cache and updates
 * file entry.
 *
 * INPUT
 *
 * vi - volume pointer
 * f - FN_FILEINT structure which file position needed to be modified
 *
 * RETURN
 *
 * error codes or zero if success
 *
 ***************************************************************************/

static int _f_abortwrite (F_VOLUME *vi, FN_FILEINT *f)
{
	int ret;

	f->mode |= FN_FILE_ABORT_FLAG; /* file is aborted for further access */

	ret=_f_emptywritecache(vi,f);

	if ((ret==F_NO_ERROR) && f->startcluster)
	{
		ret=_f_updatefileentry(vi,f,0);
		if (ret==F_NO_ERROR) return ret;
	}
	else
	{
		(void)_f_updatefileentry(vi,f,1);
		/* skip return value */
	}
#ifdef USE_MALLOC
	if (f->WrDataCache.pos) 
	{
		__free(f->WrDataCache.pos);
		f->WrDataCache.pos=NULL;
	}
#endif
	return ret;
}

/****************************************************************************
 *
 * fn_write
 *
 * write data into file
 *
 * INPUTS
 *
 * fm - multi structure pointer
 * buf - where the store data
 * size - size of items to be read
 * size_st - number of items need to be read
 * filehandle - file where to write to
 *
 * RETURNS
 *
 * with the number of written items
 *
 ***************************************************************************/

#if F_SUPPORT_TI64K
long fn_write_sub(F_MULTI *fm,const void *buf,long size,long size_st,FN_FILE *filehandle)
#else
long fn_write(F_MULTI *fm,const void *buf,long size,long size_st,FN_FILE *filehandle)
#endif
{
	F_VOLUME *vi;
	FN_FILEINT *f=_f_check_handle(filehandle);
	char *buffer=(char*)buf;
	long retsize=0,retsize2=0;
	unsigned long filesize2;
	int ret;
	char *ptr=NULL;
	long ws=size;

	/* check if file is open */
	if (!f)
	{
	 	F_SETLASTERROR_NORET(F_ERR_NOTOPEN);
		return 0;
	}

	/* check modes where write is allowed w,w+,r+,a,a+ */
	if (f->mode!=FN_FILE_WR && f->mode!=FN_FILE_A && f->mode!=FN_FILE_RDP && f->mode!=FN_FILE_WRP && f->mode!=FN_FILE_AP)
	{
	 	F_SETLASTERROR_NORET(F_ERR_NOTOPEN);
		return 0;
	}

	/* checking if drive is exist and get the volume pointer */
	ret=_f_getvolume(fm,f->drivenum,&vi);
	if (ret)
	{
	 	F_SETLASTERROR_NORET(ret);
		return 0; /* can't write */
	}

	/* get the whole size */
	size*=size_st;

	/* check if size is negative more than 2 GB is not allowed */
	if (size<0)
	{
		F_SETLASTERROR_NORET(F_ERR_UNKNOWN);
		return 0; /* size is negative */
	}

	/* check if size is zero */
	if (size==0)
	{
	   	F_SETLASTERROR_NORET(F_NO_ERROR);
		return 0; /* size is zero */
	}

	/* check if new file size reach 4GB limit, if it does it returns with EOF */
	if (f->filesize+size <= f->filesize || f->filesize+size==0)
	{
	 	F_SETLASTERROR_NORET(F_ERR_EOF);
		return 0; /* fn_write simple rejects write beyond 4GB of data */
	}

	f->datawritten=0;
	filesize2=f->filesize;
	while (size)
	{
		unsigned long wrsize=(unsigned long)size;

		/* check whether we reach the end of the file buffer */
		if (f->relpos==(vi->bootrecord.bytes_per_sector))
		{
			/* now it is full, so empty it */
			ret=_f_emptywritebuffer(vi,f,ptr);
			if (ret)
			{
				f->filesize=filesize2;

				if (_f_abortwrite(vi,f)==0)
				{
				 	F_SETLASTERROR_NORET(ret);
					return retsize2/ws;
				}

			 	F_SETLASTERROR_NORET(ret);
				return 0;
			}

#if F_MAXSEEKPOS
			_fn_updateseekpos(f);
#endif
			/* increaseing poisitions */
			f->abspos+=f->relpos;
			f->relpos=0;

			/* goto next */
			f->pos.sector++;	

			/* if remaining bytes are less then (vi->bootrecord.bytes_per_sector) then read the content of next sector */
			if (wrsize<(vi->bootrecord.bytes_per_sector))
			{
				ret=_f_getcurrsector(vi,f,NULL,0);

				if (ret)
				{
			  		if (ret!=F_ERR_EOF)
					{
						if (_f_abortwrite(vi,f)==0)
						{
						 	F_SETLASTERROR_NORET(ret);
							return retsize/ws;
						}
					 	F_SETLASTERROR_NORET(ret);
						return 0;
					}
				}
			}
		}

		/* limit datawrites up to sectossize */
		if (wrsize>=(vi->bootrecord.bytes_per_sector)-f->relpos)
		{
			wrsize=(vi->bootrecord.bytes_per_sector)-f->relpos;
		}

		/* file buffer is modified */
		f->modified=1;	

		/* check wheter whole sector is needed to be written*/
		if (wrsize==(vi->bootrecord.bytes_per_sector))
		{
			/* caching buffer for later storing */
			ptr=buffer;

			/* check if sync is needed for data content */
			if (f->state & F_FILE_ST_SYNC)
			{
				_f_syncfilescontent(f,(unsigned char*)buffer,0,(vi->bootrecord.bytes_per_sector));
			}

			/* check if this is the last data write */
			if ((unsigned long)size==wrsize)
			{
				(void)_memcpy((void *)(f->data),(void *)buffer,(vi->bootrecord.bytes_per_sector));
				f->datawritten=1;

				ret=_f_emptywritebuffer(vi,f,ptr);
				if (ret)
				{
					if (_f_abortwrite(vi,f)==0)
					{
					 	F_SETLASTERROR_NORET(ret);
						return retsize/ws;
					}
				 	F_SETLASTERROR_NORET(ret);
					return 0;
				}
			}
		}
		else
		{
			(void)_memcpy((void *)(f->data+f->relpos),(void *)buffer,(unsigned int)wrsize);

			/* check if sync is needed for data content */
			if (f->state & F_FILE_ST_SYNC)
			{
				_f_syncfilescontent(f,f->data,(int)f->relpos,(int)wrsize);
			}

			/* alloc new cluster (allocation is done only when current sector is out of current cluster) */
			ret=_f_allocnextcluster(vi,f);
			if (ret)
			{
				if (_f_abortwrite(vi,f)==0)
				{
				 	F_SETLASTERROR_NORET(ret);
					return retsize/ws;
				}
			 	F_SETLASTERROR_NORET(ret);
				return 0;
			}
		}


		buffer+=wrsize;
		size-=wrsize;
		retsize2=retsize;
		retsize+=wrsize;
		f->relpos+=wrsize;
		filesize2=f->filesize;
		if (f->filesize<f->abspos+f->relpos)
		{
			f->filesize=f->abspos+f->relpos;
		}
	}

	ret=_f_emptywritecache(vi,f);
	if (ret)
	{

#ifdef USE_MALLOC
		if (f->WrDataCache.pos) 
		{
			__free(f->WrDataCache.pos);
			f->WrDataCache.pos=NULL;
		}
#endif

		f->mode|=FN_FILE_ABORT_FLAG;

#if F_DELETE_CONTENT
		(void)_f_removechainandcontent(vi,f->startcluster);
#else
		(void)_f_removechain(vi,f->startcluster);
#endif

	 	F_SETLASTERROR_NORET(ret);
		return 0;
	}

	if (f->state & F_FILE_ST_SYNC)
	{
		ret=_f_emptywritebuffer(vi,f,NULL);
		if (ret)
		{
			(void)_f_updatefileentry(vi,f,1);

#ifdef USE_MALLOC
			if (f->WrDataCache.pos) 
			{
				__free(f->WrDataCache.pos);
				f->WrDataCache.pos=NULL;
			}
#endif

			f->mode|=FN_FILE_ABORT_FLAG;
		 	F_SETLASTERROR_NORET(ret);
			return 0;
		}

		_f_syncfiles(vi,fm,f);
	}

   	F_SETLASTERROR_NORET(F_NO_ERROR);
	return retsize/ws;
}

/****************************************************************************
 *
 * fn_write
 *
 * write data int file, this is a TI extension   for handling 64K boundary
 * problem
 *
 * INPUTS
 *
 * fm - multi structure pointer
 * buf - where the get data
 * size - size of items to be written
 * size_st - number of items need to be written
 * filehandle - file where to write to
 *
 * RETURNS
 *
 * with the number of read items
 *
 ***************************************************************************/

#if F_SUPPORT_TI64K
long fn_write(F_MULTI *fm,const void *buf,long size,long size_st,FN_FILE *filehandle)
{
	unsigned long data=(unsigned long)buf;
	long readsize=0;

	long allsize=(long)(size * size_st);

	/* check if size is zero */
	if (allsize==0)
	{
	   	F_SETLASTERROR_NORET(F_NO_ERROR);
		return 0; /* size is zero */
	}

	/* check if size is negative more than 2 GB is not allowed */
	if (allsize<0)
	{
		F_SETLASTERROR_NORET(F_ERR_UNKNOWN);
		return 0; /* size is negative */
	}

	while (allsize)
	{
		long wsize;
		long dsize=(long)(0x10000 - (data & 0xffff));
		
		if (dsize > allsize)
		{
			dsize=allsize;
		}

		wsize=fn_write_sub(fm,(void *)data,1,dsize,filehandle);
		readsize+=wsize;

		if (wsize!=dsize)
		{
			/* calculate all read size divided by item size */
			return readsize/size;
		}

		data+=dsize;
		allsize-=dsize;
	}

	F_SETLASTERROR_NORET(F_NO_ERROR);
	return size_st;
}
#endif

/****************************************************************************
 *
 * fn_seek
 *
 * moves position into given offset in given file
 *
 * INPUTS
 *
 * filehandle - FN_FILE structure which file position needed to be modified
 * offset - relative position
 * whence - where to calculate position (FN_SEEK_SET,FN_SEEK_CUR,FN_SEEK_END)
 *
 * RETURNS
 *
 * 0 - if successfully
 * other - if any error
 *
 ***************************************************************************/

int fn_seek(F_MULTI *fm,FN_FILE *filehandle,long offset,long whence)
{
	FN_FILEINT *f=_f_check_handle(filehandle);
	F_VOLUME *vi;
	int ret;

	if (!f) return F_SETLASTERROR(F_ERR_NOTOPEN);

	if (f->mode!=FN_FILE_RD && f->mode!=FN_FILE_WR && f->mode!=FN_FILE_A && f->mode!=FN_FILE_RDP && f->mode!=FN_FILE_WRP && f->mode!=FN_FILE_AP)
	{
		return F_SETLASTERROR(F_ERR_NOTOPEN);
	}

	ret=_f_getvolume(fm,f->drivenum,&vi);
	if (ret) return F_SETLASTERROR(ret);

	switch (whence)
	{
	case FN_SEEK_CUR:
		{
			unsigned long pos=f->abspos+f->relpos;
			if (offset>=0)
			{
				if ((unsigned long)(pos+offset) < pos)
				{
					return F_SETLASTERROR(F_ERR_INVALIDPOS); /* 4GB overrun on top */
				}
			}
			else
			{
				if ((unsigned long)(pos+offset) > pos)
				{
					return F_SETLASTERROR(F_ERR_INVALIDPOS); /* 4GB overrun on botton */
				}
			}
			return F_SETLASTERROR(_f_fseek(vi,fm,f,f->abspos+f->relpos+offset,0));
		}
	case FN_SEEK_END:
		{
			unsigned long pos=f->filesize;
			if (offset>=0)
			{
				if ((unsigned long)(pos+offset) < pos)
				{
					return F_SETLASTERROR(F_ERR_INVALIDPOS); /* 4GB overrun on top */
				}
			}
			else
			{
				if ((unsigned long)(pos+offset) > pos)
				{
					return F_SETLASTERROR(F_ERR_INVALIDPOS); /* 4GB overrun on botton */
				}
			}
			return F_SETLASTERROR(_f_fseek(vi,fm,f,f->filesize+offset,0));
		}
	case FN_SEEK_SET:
		{
			if (offset<0)
			{
				return F_SETLASTERROR(F_ERR_INVALIDPOS); /* cannot seek into negative position */
			}

			return F_SETLASTERROR(_f_fseek(vi,fm,f,(unsigned long)offset,0));
		}
	default:
		return F_SETLASTERROR(F_ERR_NOTUSEABLE);
	}
}

/****************************************************************************
 *
 * fn_tell
 *
 * Tells the current position of opened file
 *
 * INPUTS
 *
 * fm - multi structure
 * filehandle - which file needs the position
 *
 * RETURNS
 *
 * position in the file from start or -1 if any error
 *
 ***************************************************************************/

long fn_tell(F_MULTI *fm,FN_FILE *filehandle)
{
	FN_FILEINT *f=_f_check_handle(filehandle);

	if (!f)
	{
		F_SETLASTERROR_NORET(F_ERR_NOTOPEN);
		return -1;
	}

	if (f->mode!=FN_FILE_RD && f->mode!=FN_FILE_WR && f->mode!=FN_FILE_A && f->mode!=FN_FILE_RDP && f->mode!=FN_FILE_WRP && f->mode!=FN_FILE_AP)
	{
		F_SETLASTERROR_NORET(F_ERR_NOTOPEN);
		return -1;
	}

	F_SETLASTERROR_NORET(F_NO_ERROR);
	return (long)(f->abspos+f->relpos);
}

/****************************************************************************
 *
 * fn_eof
 *
 * Tells if the current position is end of file or not
 *
 * INPUTS
 *
 * fm - multi structure
 * filehandle - which file needs the checking
 *
 * RETURNS
 *
 * 0 - if not EOF
 * other - if EOF or invalid file handle
 *
 ***************************************************************************/

int fn_eof(F_MULTI *fm,FN_FILE *filehandle)
{
	FN_FILEINT *f=_f_check_handle(filehandle);

	if (!f) return F_SETLASTERROR(F_ERR_NOTOPEN);  /* if error */
	if (f->abspos+f->relpos<f->filesize) return F_SETLASTERROR(F_NO_ERROR);

	return F_SETLASTERROR(F_ERR_EOF);/* EOF */
}

/****************************************************************************
 *
 * fn_rewind
 *
 * set the fileposition in the opened file to the begining
 *
 * INPUTS
 *
 * filehandle - which file needs to be rewinded
 *
 * RETURNS
 *
 * error code or zero if successful
 *
 ***************************************************************************/

int fn_rewind(F_MULTI *fm,FN_FILE *filehandle)
{
	/* fn_seek sets the last error code */
	return fn_seek(fm, filehandle, 0L, FN_SEEK_SET);
}

/****************************************************************************
 *
 * fn_putc
 *
 * write a character into file
 *
 * INPUTS
 *
 * fm - multi structure pointer
 * ch - what to write into file
 * filehandle - file where to write
 *
 * RETURNS
 *
 * with the written character or -1 if any error
 *
 ***************************************************************************/

int fn_putc(F_MULTI *fm,int ch,FN_FILE *filehandle)
{
	unsigned char tmpch=(unsigned char)(ch);

	/* fn_write sets the last error code */
	if (fn_write(fm,&tmpch,1,1,filehandle)==1) return tmpch;

	return -1;
}

/****************************************************************************
 *
 * fn_getc
 *
 * get a character from file
 *
 * INPUTS
 *
 * fm - multi structure pointer
 * filehandle - file where to read from
 *
 * RETURNS
 *
 * with the read character or -1 if read was not successfully
 *
 ***************************************************************************/

int fn_getc(F_MULTI *fm,FN_FILE *filehandle)
{
	unsigned char ch;

	/* fn_read sets the last error code */
	if (fn_read(fm,&ch,1,1,filehandle)==1) return ch;

	return -1;
}

/****************************************************************************
 *
 * f_enterFS
 *
 * Put taskID into the task list, this function has to be called when a new
 * task wants to use FS (before call any FS function)
 *
 * RETURNS
 *
 * errorcode or 0 if successful
 *
 ***************************************************************************/

#if (!FN_CAPI_USED)
#if (!F_REMOVEENTERFS)
int f_enterFS()
#else
static int _f_enterFS(long ID)
#endif
{
	int a;

#if (!F_REMOVEENTERFS)
	long ID=fn_gettaskID();

	/* check if taskid is exist */
	if (!fnGetTask(0)) return F_NO_ERROR; /* already in */
#endif

	/* wait until global mutex is released */
	for (;;)
	{
		if (!f_mutex_get(&fat_gmutex)) break;
	}

	/* alloc new taskid */
	for (a=0; a<FN_MAXTASK;a++)
	{
#ifdef USE_MALLOC
		/* check for empty space*/
		if (g_multi[a]==NULL)
		{	
			F_MULTI *m=(F_MULTI *)__malloc(sizeof(F_MULTI));
			if (!m)
			{
				(void)f_mutex_put(&fat_gmutex);
				return F_ERR_ALLOCATION;
			}

			m->ID=ID;	  /* set task ID */
#if USE_TASK_SEPARATED_CWD
			m->f_curdrive=FN_CURRDRIVE;
#endif
			m->lasterror=0;

			g_multi[a]=m;

#if USE_TASK_SEPARATED_CWD
			{
				int b;

				for (b=0; b<FN_MAXVOLUME; b++)
				{
					g_multi[a]->f_vols[b].cwd[0]=0;
				}
			}
#endif
			g_multi[a]->pmutex=0;

			(void)f_mutex_put(&fat_gmutex);
			return F_NO_ERROR; /* free to use */
		}
#else
		/* check for empty space*/
		if (!g_multi[a].ID)
		{	
			g_multi[a].ID=ID;	  /* set task ID */
#if USE_TASK_SEPARATED_CWD
			g_multi[a].f_curdrive=FN_CURRDRIVE;
#endif
			g_multi[a].lasterror=0;

#if USE_TASK_SEPARATED_CWD
			{
				int b;

				for (b=0; b<FN_MAXVOLUME; b++)
				{
					g_multi[a].f_vols[b].cwd[0]=0;
				}
			}
#endif

			g_multi[a].pmutex=0;

			(void)f_mutex_put(&fat_gmutex);
			return F_NO_ERROR; /* free to use */
		}
#endif
	}

	(void)f_mutex_put(&fat_gmutex);
	return F_ERR_NOMOREENTRY; /* no space to set up new task */
}
#endif

/****************************************************************************
 *
 * f_releaseFS
 *
 * Release used task specified F_MULTI structure
 *
 * INPUT
 *
 * ID - which ID need to be released
 *
 ***************************************************************************/

#if (!FN_CAPI_USED)
void f_releaseFS(long ID)
{
	int a;

	for (a=0; a<FN_MAXTASK;a++)
	{
#ifdef USE_MALLOC
		if (g_multi[a] && g_multi[a]->ID==ID)
		{
 	  		__free(g_multi[a]);
 	  		g_multi[a]=NULL;
			return;
		}
#else
 		if (g_multi[a].ID==ID)
		{
			g_multi[a].ID=0;
			return;
		}
#endif
	}
}
#endif

/****************************************************************************
 *
 * fnGetTask
 *
 * Searching for an entry where the task infos are
 *
 * INPUTS
 *
 * fm - where to store information
 *
 * RETURNS
 *
 * 0 - if found
 * 1 - if no more entry
 *
 ***************************************************************************/

#if (!FN_CAPI_USED)
int fnGetTask(F_MULTI **fm)
{
	int a;
	long ID=fn_gettaskID();

	/* check if taskid is exist */
	for (a=0; a<FN_MAXTASK;a++)
	{
#ifdef USE_MALLOC
		if (g_multi[a] && g_multi[a]->ID==ID)
		{  /* check if exist */
			if (fm)
			{
				*fm=g_multi[a];
				g_multi[a]->pmutex=0;
			}
			return 0; /* free to use */
		}
#else
		if (g_multi[a].ID==ID)
		{  /* check if exist */
			if (fm)
			{
				*fm=&g_multi[a];
				g_multi[a].pmutex=0;
			}
			return 0; /* free to use */
		}
#endif
	}
#if F_REMOVEENTERFS
	/* backward compatibility */
	return _f_enterFS(ID); 
#else
	return 1; /* not found */
#endif
}
#endif

/****************************************************************************
 *
 * _f_mutex_get
 *
 * Requests for a mutex.
 *
 * fm - multi structure pointer
 * vi - volume info
 *
 * RETURNS
 *
 * 0 - if access granted
 * 1 - if busy
 *
 ***************************************************************************/

int _f_mutex_get(F_MULTI *fm,F_VOLUME *vi)
{

	/* check if driver exist */
	if (!vi->driver)
	{
		return F_ERR_INVALIDDRIVE;
	}

	 /* check if we already locked it */
	if (fm->pmutex==&vi->driver->mutex)
	{
		return F_NO_ERROR;
	}

	if (fm)
	{
		/* lock it */
		if (!f_mutex_get(&vi->driver->mutex))
		{
			fm->pmutex=&vi->driver->mutex; /* for releasing after finished whole access */
			return F_NO_ERROR;
		}
	}

	return F_ERR_BUSY;
}

/****************************************************************************
 *
 * _f_mutex_put
 *
 * Release used mutex
 *
 * INPUTS
 *
 * fm - multi structure pointer
 *
 ***************************************************************************/

#if (!FN_CAPI_USED)
void _f_mutex_put(F_MULTI *fm)
{
	if (fm)
	{
		if (fm->pmutex)
		{
			(void)f_mutex_put(fm->pmutex); /* release it */
			fm->pmutex=0;
		}
	}
}
#endif

/****************************************************************************
 *
 * fn_get_oem
 *
 * Get OEM name
 *
 * INPUTS
 *
 * fm - multi structure pointer
 * drivenum - drivenumber
 * str - where to store information
 * maxlen - length of the buffer
 *
 * RETURN
 *
 * errorcode or zero if successful
 *
 ***************************************************************************/

int fn_get_oem (F_MULTI *fm, int drivenum, char *str, long maxlen)
{
	F_VOLUME *vi;
	int ret;
	int i;

	ret=_f_getvolume(fm,drivenum,&vi);
	if (ret) return F_SETLASTERROR(ret);

	if (maxlen)
	{
		if ((int)maxlen>(int)sizeof(vi->bootrecord.OEM_name)) maxlen=sizeof(vi->bootrecord.OEM_name);
		else maxlen--;

		for (i=0;i<maxlen;i++)
		{
			str[i]=(char)vi->bootrecord.OEM_name[i];
		}
		str[i]=0;
	}

	return F_SETLASTERROR(F_NO_ERROR);
}

/****************************************************************************
 *
 * fn_ftruncate
 *
 * truncate a file to a specified length, filepointer will be set to the
 * end
 *
 * INPUTS
 *
 * fm - multi structure pointer
 * filehandle - which file need to be truncated
 * length - length of new file
 *
 * RETURNS
 *
 * error code or zero if successful
 *
 ***************************************************************************/

int	fn_ftruncate(F_MULTI *fm,FN_FILE *filehandle,unsigned long length)
{
	/* fn_seek sets the last error code */
	int ret=fn_seek(fm,filehandle,(long)length,FN_SEEK_SET);
	if (ret) return ret;

	/* fn_seteof sets the last error code */
	return fn_seteof(fm,filehandle);
}

/****************************************************************************
 *
 * fn_seteof
 *
 * set end of file at the current position
 *
 * INPUTS
 *
 * fm - multi structure pointer
 * filehandle - file where to read from
 *
 * RETURNS
 *
 * errorcode or zero if successful
 *
 ***************************************************************************/

int fn_seteof(F_MULTI *fm,FN_FILE *filehandle)
{
	FN_FILEINT *f=_f_check_handle(filehandle);
	F_VOLUME *vi;
	int ret;
	unsigned long cluster;

	if (!f) return F_SETLASTERROR(F_ERR_NOTOPEN);

	if (f->mode!=FN_FILE_WR && f->mode!=FN_FILE_RDP && f->mode!=FN_FILE_WRP )
	{
		return F_SETLASTERROR(F_ERR_NOTOPEN);
	}

	ret=_f_getvolume(fm,f->drivenum,&vi);
	if (ret) return F_SETLASTERROR(ret);


	if (f->startcluster)
	{
		ret=_f_getclustervalue(vi,f->pos.cluster,&cluster);
		if (ret) return F_SETLASTERROR(ret);

#if F_DELETE_CONTENT
		ret=_f_removechainandcontent(vi,cluster);
		if (ret) return F_SETLASTERROR(ret);
#else
		ret=_f_removechain(vi,cluster);
		if (ret) return F_SETLASTERROR(ret);
#endif

		ret=_f_setclustervalue(vi,f->pos.cluster,F_CLUSTER_LAST);
		if (ret) return F_SETLASTERROR(ret);

		f->filesize=f->abspos+f->relpos;
	}

	if (!f->filesize)
	{
		if (f->startcluster>=2 && f->startcluster<F_CLUSTER_RESERVED)
		{
#if F_DELETE_CONTENT
			ret=_f_removechainandcontent(vi,f->startcluster);
			if (ret) return F_SETLASTERROR(ret);
#else
			ret=_f_removechain(vi,f->startcluster);
			if (ret) return F_SETLASTERROR(ret);
#endif
		}
		f->startcluster=0;
		f->pos.sector=0;
		f->pos.sectorend=0;
		f->modified=0; /* no need to save */
	}

#if F_MAXSEEKPOS
	_fn_removeseekpos(f);
#endif

	return F_SETLASTERROR(F_NO_ERROR);
}

/****************************************************************************
 *
 * _fn_initseekdivisor
 *
 * Initializing seek table with empty (-1) entries
 *
 * INPUTS
 *
 * f - file pointer
 * vi - volume pointer
 *
 ***************************************************************************/

#if F_MAXSEEKPOS
void _fn_initseekdivisor(FN_FILEINT *f,F_VOLUME *vi)
{
	long clustersize;
	long tmp;
	long seekdivizor;
	int a;

	clustersize=vi->bootrecord.sector_per_cluster;
	clustersize*=(vi->bootrecord.bytes_per_sector);

	tmp=(long)(f->filesize/F_MAXSEEKPOS);
	tmp=(tmp+(clustersize>>1))/clustersize;
	if (!tmp) tmp=1;

	seekdivizor=tmp*clustersize;

	f->seekshift=9;
	tmp=512;
	for (a=0; a<31; a++)
	{
		if (tmp>=seekdivizor) break;
		tmp<<=1;
		f->seekshift++;
	}

	for (a=0; a<F_MAXSEEKPOS; a++)
	{
		f->seekpos[a]=-1;  /* entry is not initiated */
	}
}
#endif

/****************************************************************************
 *
 * _fn_updateseekpos
 *
 * updating seek table according f->abspos
 *
 * INPUTS
 *
 * f - file pointer
 *
 ***************************************************************************/

#if F_MAXSEEKPOS
void _fn_updateseekpos(FN_FILEINT *f)
{
	long pos;

	pos=(long)(f->abspos >> f->seekshift);
	pos--;

	if (pos>=0 && pos<F_MAXSEEKPOS)
	{
		if (f->seekpos[pos]==-1)
		{
			f->seekpos[pos]=(long)f->pos.cluster;
			f->seekprev[pos]=(long)f->pos.prevcluster;
		}
	}
}
#endif

/****************************************************************************
 *
 * _fn_removeseekpos
 *
 * removing entries from seek table
 *
 * INPUTS
 *
 * f - filepointer
 *
 ***************************************************************************/

#if F_MAXSEEKPOS
void _fn_removeseekpos(FN_FILEINT *f)
{
	long pos;

	pos=(long)(f->filesize >> f->seekshift);

	if ((pos << f->seekshift) == (long)f->filesize) pos--;	 /* remove entry if its equal also */

	if (pos<0) pos=0;

	for (;pos<F_MAXSEEKPOS; pos++)
	{
		f->seekpos[pos]=-1;
	}
}
#endif

/****************************************************************************
 *
 * _f_findopensize
 *
 * finding file in opened file and return its size
 *
 * INPUTS
 *
 * osize - pointer where to store size of the opened file
 * drivenum - which drive on
 * pos - position structure
 *
 * RETURNS
 *
 * 1 - if found and osize is filled
 * 0 - not found
 *
 ***************************************************************************/

int _f_findopensize(unsigned long *osize,int drivenum,F_POS *pos)
{
	/* set F_FINDOPENFILESIZE to 0 if filelength needs to return with 0 for an opened file  */
	/* other case filelength functions can return with opened file length also */
#if F_FINDOPENFILESIZE
	int a;

	for (a=0; a<F_MAXFILES; a++)
	{
		FN_FILEINT *f=&f_filesystem.files[a];

		if (f->mode!=FN_FILE_CLOSE) /* if its not closed */
		{	/* same drive? same direntry? same dirsector? */
			if (drivenum==f->drivenum && f->dirpos.pos==pos->pos && f->dirpos.sector==pos->sector)
			{
				if (f->mode==FN_FILE_WR || f->mode==FN_FILE_WRP ||
					f->mode==FN_FILE_A || f->mode==FN_FILE_AP ||
					f->mode==FN_FILE_RDP)
				{
					*osize=f->filesize; /* get from opened file */
					return 1;
				}
			}
		}
	}
#endif

	return 0;
}

/****************************************************************************
 *
 * _f_createfullname
 *
 * create full name
 *
 * INPUTS
 *
 * buffer - where to create
 * buffersize - size of the buffer
 * drivenum - drive number
 * path - path of the file
 * filename - file name
 * fileext - file extension
 *
 * RETURNS
 *
 * 1 - if found and osize is filled
 * 0 - not found
 *
 ***************************************************************************/

#if (!F_LONGFILENAME)
int _f_createfullname(char *buffer,int buffersize,int drivenum,char *path,char *filename,char *fileext)
{
	char *fullname=buffer;
	int a;

	/* adding drive letter */
	if (buffersize<3)
	{
		return 1;
	}
	*fullname++=(char)(drivenum+'A');
	*fullname++=':';
	*fullname++=(char)F_SEPARATORCHAR;
	buffersize-=3;

	/* adding path */
	if (path[0])
	{
		for (;;)
		{
			char ch=*path++;

			if (!ch) 
			{
				break;
			}

			if (buffersize<=0)
			{
				return 1;
			}

			*fullname++=ch;
			buffersize--;
		}

		/* adding separator */
		if (buffersize<=0)
		{
			return 1;
		}
		*fullname++=(char)F_SEPARATORCHAR;

	}

	/* adding name */
	for (a=0;a<F_MAXNAME;a++)
	{
		char ch=*filename++;

		if ((!ch) || (ch==32))
		{
			break;
		}

		if (buffersize<=0)
		{
			return 1;
		}

		*fullname++=ch;
		buffersize--;
	}

	/* adding ext*/
	if (fileext[0] && fileext[0]!=32)
	{
		/* adding dot */
		if (!buffersize)
		{
			return 1;
		}
		*fullname++='.';

		for (a=0;a<F_MAXEXT;a++)
		{
			char ch=*fileext++;

			if ((!ch) || (ch==32))
			{
				break;
			}

			if (buffersize<=0)
			{
				return 1;
			}

			*fullname++=ch;
			buffersize--;
		}
	}

	/* adding terminator */
	if (buffersize<=0)
	{
		return 1;
	}
	*fullname++=0;

	return 0;
}

#else

int _f_createfullname(char *buffer,int buffersize,int drivenum,char *path,char *lname)
{
	char *fullname=buffer;

	/* adding drive letter */
	if (buffersize<3)
	{
		return 1;
	}
	*fullname++=(char)(drivenum+'A');
	*fullname++=':';
	*fullname++=(char)F_SEPARATORCHAR;
	buffersize-=3;

	/* adding path */
	if (path[0])
	{
		for (;;)
		{
			char ch=*path++;

			if (!ch) 
			{
				break;
			}

			if (buffersize<=0)
			{
				return 1;
			}

			*fullname++=ch;
			buffersize--;
		}

		/* adding separator */
		if (buffersize<=0)
		{
			return 1;
		}
		*fullname++=(char)F_SEPARATORCHAR;

	}

	/* adding name */
	for (;;)
	{
		char ch=*lname++;

		if (!ch) 
		{
			break;
		}

		if (buffersize<=0)
		{
			return 1;
		}

		*fullname++=ch;
		buffersize--;
	}

	/* adding terminator */
	if (buffersize<=0)
	{
		return 1;
	}
	*fullname++=0;

	return 0;
}
#endif

/****************************************************************************
 *
 * fn_setlasterror
 *
 * setting errorcode in multi structure, maybe functions is not used
 * depending on thse settings in udefs_f.h
 * fn_setlasterror differs only it has no return value.
 *
 * INPUTS
 *
 * fm - multi structure where to set
 * errorcode - errorcode to be set
 *
 * RETURNS
 *
 * simple return with errorcode
 *
 ***************************************************************************/

int fn_setlasterror (F_MULTI *fm, int errorcode)
{
	fm->lasterror=errorcode;
	return errorcode;
}

void fn_setlasterror_noret (F_MULTI *fm, int errorcode)
{
	fm->lasterror=errorcode;
}

/****************************************************************************
 *
 * end of common.c
 *
 ***************************************************************************/
