/**
 * Host-side USB direct command example
 * 
 * Prerequisite : sudo apt-get install libusb-dev
 * Compilation  : gcc -o usbcmd usbcmd.c -lusb
 * Execution    : sudo ./usbcmd
 * 
 * jacques.gangloff@unistra.fr, 5/9/13
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <usb.h>

#define EV3_USB_TIMEOUT             2000    // Milliseconds
#define EV3_USB_NAME_LENGTH         32

/* These informations can be obtained with lsusb -v */
#define EV3_INTERFACE_NUMBER        0
#define EV3_CONFIGURATION_NB        1
#define EV3_EP_OUT                  0x01
#define EV3_EP_IN                   0x81

/* Error codes */
#define EV3_OK                      0
#define EV3_NOT_PRESENT             1
#define EV3_CONFIGURATION_ERROR     2
#define EV3_IN_USE                  3
#define EV3_USB_WRITE_ERROR         4
#define EV3_USB_READ_ERROR          5


enum ev3_usb_ids {
  VENDOR_LEGO   = 0x0694,
  PRODUCT_EV3   = 0x0005
};

struct ev3_t {
  struct usb_device     *dev;
  struct usb_dev_handle *hdl;
};

typedef unsigned char               ev3_error_t;
typedef struct ev3_t                ev3_t;


ev3_error_t ev3_init( ev3_t **ev3 )  {

  usb_init( );
  *ev3 = calloc( 1, sizeof( **ev3 ) );

  return EV3_OK;
}


ev3_error_t ev3_find( ev3_t *ev3 )  {
  struct usb_bus *busses, *bus;

  usb_find_busses( );
  usb_find_devices( );

  busses = usb_get_busses( );

  for ( bus = busses; bus != NULL; bus = bus->next )  {
    struct usb_device *dev;

    for ( dev = bus->devices; dev != NULL; dev = dev->next )  {
      if (  dev->descriptor.idVendor == VENDOR_LEGO &&
            dev->descriptor.idProduct == PRODUCT_EV3 )  {
        ev3->dev = dev;
        return EV3_OK;
      }
    }
  }

  return EV3_NOT_PRESENT;
}


ev3_error_t ev3_open( ev3_t *ev3 )  {
  char bound_driver_name[EV3_USB_NAME_LENGTH];
  int ret;

  ev3->hdl = usb_open( ev3->dev );

  /* Detach possible kernel driver bound to interface */
  if ( usb_get_driver_np( ev3->hdl, EV3_INTERFACE_NUMBER, bound_driver_name, EV3_USB_NAME_LENGTH ) == 0 )  {
     if ( usb_detach_kernel_driver_np( ev3->hdl, EV3_INTERFACE_NUMBER ) < 0 )  {
      usb_close( ev3->hdl );
      return EV3_IN_USE;
    }
  }  

  ret = usb_set_configuration( ev3->hdl, EV3_CONFIGURATION_NB );

  if ( ret < 0 )  {
    usb_close( ev3->hdl );
    return EV3_CONFIGURATION_ERROR;
  }

  ret = usb_claim_interface( ev3->hdl, EV3_INTERFACE_NUMBER );

  if ( ret < 0 )  {
    usb_close( ev3->hdl );
    return EV3_IN_USE;
  }

  return EV3_OK;
}


ev3_error_t ev3_close( ev3_t *ev3 )  {

  usb_release_interface( ev3->hdl, EV3_INTERFACE_NUMBER );
  usb_close( ev3->hdl );
  free( ev3 );

  return EV3_OK;
}

ev3_error_t ev3_send_buf( ev3_t *ev3, char *buf, int len )  {

  int ret = usb_bulk_write( ev3->hdl, EV3_EP_OUT, buf, len, EV3_USB_TIMEOUT );

  if ( ret < 0 )
    return EV3_USB_WRITE_ERROR;

  return EV3_OK;
}


ev3_error_t ev3_send_str( ev3_t *ev3, char *str )  {

  return ev3_send_buf( ev3, str, strlen( str ) );
}


ev3_error_t ev3_recv_buf( ev3_t *ev3, char *buf, int len )  {

  int ret = usb_bulk_read( ev3->hdl, EV3_EP_IN, buf, len, EV3_USB_TIMEOUT );

  if ( ret < 0 )
    return EV3_USB_READ_ERROR;

  return EV3_OK;
}

int main( void )  {
  ev3_t         *ev3;
  ev3_error_t   err;

  /* See file "c_com.h" in "ev3sources/lms2012/c_com" (https://github.com/mindboards/ev3sources) */
  char          ev3_motor_start[] = { 0x0A, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 
			0x9A, 0x00, 0x00, 0x00, 0x02 };
  char          ev3_motor_stop[]  = "\x09\x00\x01\x00\x80\x00\x00\xA3\x00\x06\x00";
  char recv[6];

  /* Initialize libusb */
  ev3_init( &ev3 );

  /* Look for an EV3 in USB devices list */
  err = ev3_find( ev3 );
  if ( err )  {
    if ( err == EV3_NOT_PRESENT )
      fprintf( stderr, "EV3 not found. Is it properly plugged in USB port?\n" );
    else
      fprintf( stderr, "Error while scanning for EV3.\n" );
    exit( 1 );
  }

  /* Open EV3 USB port */
  err = ev3_open( ev3 );
  if ( err )  {
    if ( err == EV3_CONFIGURATION_ERROR )
      fprintf( stderr, "EV3 USB configuration error.\n" );
    else
      fprintf( stderr, "Error while opening EV3 USB port.\n" );
    exit( 2 );
  }

  /* Start motor on port A at 20% */
  ev3_send_buf( ev3, ev3_motor_start, sizeof( ev3_motor_start ) );

  /* Sleep for some time */
  sleep( 5 );

  /* Stop motor on port A, floating */
	ev3_recv_buf( ev3, recv, sizeof( recv ) );
	
	int i = 0;
	printf("read:");
	for(i = 0;i<sizeof(recv);i++)
		printf(" %4d",recv[i]);
	printf("\n");

  /* Close EV3 USB port */
  ev3_close( ev3 );
}
