#ifndef __EV3_USBCOM_CPP
#define __EV3_USBCOM_CPP

/* constructor */
ev3_USBCom::ev3_USBCom(unsigned char Motor_L, unsigned char Motor_R) {

			err = EV3_NOT_INITIALIZED;
			/* default command arrays (see c_com.h) */
			eng_lr = { 	0x0D, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00,
						0xA5, 0x00, (unsigned char)(Motor_L | Motor_R), 0x81, 0x00 /*speed*/,
						0xA6, 0x00, (unsigned char)(Motor_L | Motor_R) };
			eng_left = { 	0x0D, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00,
							0xA5, 0x00, Motor_L, 0x81, 0x00,
							0xA6, 0x00, Motor_L };
			eng_right = { 	0x0D, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00,
							0xA5, 0x00, Motor_R, 0x81, 0x00,
							0xA6, 0x00, Motor_R };
			eng_stop = { 	0x09, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00,
							0xA3, 0x00, (unsigned char)(Motor_L | Motor_R), 0x00 };
			sensorMode = { 	0x0B, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00,
							0x9A, 0x00, 0x00 /*Port 0-3*/, 0x00, 0x00 /*mode*/, 0x60 };

			/* deprecated command arrays
			eng_time = { 0x0D, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00,
				0xAF, 0x00, (unsigned char)(Motor_L | Motor_R), 0x0A, 0xF0, 0xFF, 0xF0, 0x00 };
			eng_step = { 0x0D, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00,
				0xAE, 0x00, (unsigned char)(Motor_L | Motor_R), 0x0A, 0x70, 0xFF, 0x70, 0x00};
			eng_sync = { 0x0C, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00,
				0xB1, 0x00, (unsigned char)(Motor_L | Motor_R), 0x00, 0x0F, 0xFF, 0x00 };
			*/
			
			/* init, find and open connection to EV3 */
			if(err == EV3_NOT_INITIALIZED) {
				err = ev3_init(&ev3);
				if(err == EV3_NOT_PRESENT) {
					err = ev3_find(ev3);
					if(err == EV3_USB_WAIT_TO_CONNECT) {
						err = ev3_open(ev3);
					}
				}
			}

} /* constructor */


unsigned char ev3_USBCom::convertSpeed(int s) {
	unsigned char r = 0;
	
	/* check if argument is valid */
	if( s >= -100 && s <= 100 ) {
		r = (unsigned char)(s);
	}
	
	/* limit invalid values */
	else if( s < -100 )
		r = -100;
	else if( s > 100 )
		r = 100;

	return r;
} // convertSpeed


ev3_error_t ev3_USBCom::close() {
	return 0;//ev3_close(ev3);
} // close


ev3_error_t ev3_USBCom::status(bool echo) {
	
	/* check if ncurses is used */
	#ifdef __NCURSES_H
	if(echo) {
		mvprintw( 0, 2, "Status:");
		switch(err) {
			case EV3_OK:
				mvprintw( 0, 10, "EV3 verbunden");
				break;
			case EV3_NOT_PRESENT:
				mvprintw( 0, 10, "EV3 nicht verbunden");
				break;
			case EV3_CONFIGURATION_ERROR:
				mvprintw( 0, 10, "Konfigurationsfehler");
				break;
			case EV3_IN_USE:
				mvprintw( 0, 10, "EV3 war noch aktiv");
				break;
			case EV3_USB_WRITE_ERROR:
				mvprintw( 0, 10, "USB Ausgabefehler");
				break;
			case EV3_USB_READ_ERROR:
				mvprintw( 0, 10, "USB Eingabefehler");
				break;
			case EV3_USB_CONNECTION_CLOSED:
				mvprintw( 0, 10, "USB Verbindung geschlossen");
				break;
			case EV3_USB_WAIT_TO_CONNECT:
				mvprintw( 0, 10, "Warte auf Verbindungsaufbau");
				break;
			default:
				mvprintw( 0, 10, "Unbekannter Fehler: %d", err);
		}
	}
	#endif
	
	/* return error code of EV3 */
	return err;
} //status


ev3_error_t ev3_USBCom::drive(drive_t drv) {
	// check for available EV3
	if(err == EV3_OK) {

		// check for active brakes
		if(drv.brakes) {

			// set brake mode
			if(drv.brake_mode)
				eng_stop[10] = 0x01;
			else
				eng_stop[10] = 0x00;

			// send command
			err = ev3_send_buf( ev3, eng_stop, sizeof(eng_stop) );
			// and exit
			return err;
		}
		// check for active steering
		else if(drv.turn) {

			bool right = false; // direction
			int sp_high = 0;	// speed on high side
			int sp_low = 0;		// speed on low side
			
			/* limit turn value */
			if(drv.turn > 100)
				drv.turn = 100;
			else if(drv.turn < -100)
				drv.turn = -100;
			
			/* apply acc steering if needed */
			if(drv.acc_steering)
				drv.turn *= 2;

			// set direction
			if(drv.turn < 0) {
				right = true;
				drv.turn *= -1;
			}

			// handle uneven integer
			if(drv.turn%2) {
				sp_high += 1;
				if( drv.turn > 0 )
					drv.turn -= 1;
				else
					drv.turn += 1;
			}

			// split turn value between both sides
			sp_high += drv.turn / 2;
			sp_low -= drv.turn / 2;
			
			/* set turn to original value */
			if(drv.acc_steering)
				drv.turn /= 2;

			// add current speed
			if(drv.speed >= 0) {
				sp_high += drv.speed;
				sp_low += drv.speed;
			} else {
				sp_high -= drv.speed;
				sp_low -= drv.speed;
			}

			// correct high values
			if(sp_high > 100) {
				sp_low -= sp_high - 100;
				sp_high = 100;
			} else if(sp_high < -100) {
				sp_low += sp_high + 100;
				sp_high = -100;
			}

			// correct low values
			if(sp_low <= -100)
				sp_low = -100;
			else if(sp_low >= 100)
				sp_low = 100;

			// correct direction
			if(drv.speed < 0) {
				sp_high *= -1;
				sp_low *= -1;
			}

			// set direction to engines
			if(right) {
				eng_right[11] = convertSpeed(sp_low);
				eng_left[11] = convertSpeed(sp_high);
			} else {
				eng_right[11] = convertSpeed(sp_high);
				eng_left[11] = convertSpeed(sp_low);
			}

			// send first command...
			err = ev3_send_buf( ev3, eng_right, sizeof(eng_right) );
			// ...check for error...
			if(err == EV3_OK){
				// ...wait for the EV3...
				usleep(500);
				// ...and send the second command
				err = ev3_send_buf( ev3, eng_left, sizeof(eng_left) );
			}
		}
		// check if we are really doing something
		else if(drv.speed) {
			// set same speed to both engines
			eng_lr[11] = convertSpeed(drv.speed);
			// send command
			err = ev3_send_buf( ev3, eng_lr, sizeof(eng_lr));
		}
		// else coast
		else {
			// set brake command to coast
			eng_stop[10] = 0x00;
			// send command
			err = ev3_send_buf( ev3, eng_stop, sizeof(eng_stop) );
		}

	} // if !err

	return err;
} // drive_t drv


ev3_error_t ev3_USBCom::drive(int speed, int turn) {

	if(err == EV3_OK) {
		if(turn != 0) {
			bool right = true;
			int sp_high = 0;
			int sp_low = 0;

			if(turn < 0) {
				right = false;
				turn *= -1;
			}

			if(turn%2)
				sp_high += 1;

			sp_high += turn / 2;
			sp_low -= turn / 2;

			if(speed >= 0) {
				sp_high += speed;
				sp_low += speed;
			} else {
				sp_high -= speed;
				sp_low -= speed;
			}

			if(sp_high > 100) {
				sp_low -= sp_high - 100;
				sp_high = 100;
			} else if(sp_high <= -100) {
				sp_low += sp_high + 100;
				sp_high = -100;
			}

			if(sp_low < -100)
				sp_low = -100;
			else if(sp_low > 100)
				sp_low = 100;

			if(speed < 0) {
				sp_high *= -1;
				sp_low *= -1;
			}

			if(!right) {
				eng_right[11] = convertSpeed(sp_low);
				eng_left[11] = convertSpeed(sp_high);
			} else {
				eng_right[11] = convertSpeed(sp_high);
				eng_left[11] = convertSpeed(sp_low);
			}

			err = ev3_send_buf( ev3, eng_right, sizeof(eng_right) );
			if(err == EV3_OK){
				usleep(500);
				err = ev3_send_buf( ev3, eng_left, sizeof(eng_left) );
			}
		}
		else {
			eng_lr[10] = convertSpeed(speed);
			err = ev3_send_buf( ev3, eng_lr, sizeof(eng_lr));
		}


	} // if !err

	return err;
} // drive


/* deprecated drive functions
ev3_error_t ev3_USBCom::driveSync(int speed, int turn, int ms) {
	if(!err) {
		eng_sync[10] = convertSpeed(speed);
		//eng_sync[11] = (unsigned char)(turn);
		//printf("driveSync turn: %d\n", (unsigned char)(turn));
		//eng_sync[12] = (unsigned char)(ms);
		err = ev3_send_buf( ev3, eng_sync, sizeof(eng_sync) );
	}

	return err;
} // driveSync


ev3_error_t ev3_USBCom::driveTime(int speed, int turn, int time) {
	if(!err) {
		eng_time[10] = convertSpeed(speed);
		eng_time[11] = (char)(turn);
		eng_time[12] = (char)(time);

		err = ev3_send_buf( ev3, eng_time, sizeof(eng_time) );
	}

	return err;
} // driveTime


ev3_error_t ev3_USBCom::driveStep(int speed, int turn, int step) {
	if(!err) {
		eng_time[10] = convertSpeed(speed);
		err = ev3_send_buf( ev3, eng_step, sizeof(eng_step) );
	}

	return err;
} // driveStep
*/


ev3_error_t ev3_USBCom::stop(bool brakes) {
	
	/* check brake mode and set matching byte */
	if(brakes)
		eng_stop[10] = 0x01;
	else
		eng_stop[10] = 0x00;
	
	/* send command and return error code */
	err = ev3_send_buf( ev3, eng_stop, sizeof(eng_stop) );
	return err;
} // stop


int ev3_USBCom::readSensor(int sensorPort) {
	unsigned char tx[] = { 0x0B, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00,
		0x9A, 0x00, 0x00, 0x00, 0x00, 0x60 };
		//0B00xxxx 0001009A00 00 000060
	char rx[10];

	//rx[5] = 0x00;
	for(unsigned int i = 0; i<sizeof(rx); i++) {
		rx[i] = 0x00;
	}

	err = ev3_send_buf( ev3, tx, sizeof(tx));
	if(err != EV3_OK)
		return -1;
	err = ev3_recv_buf(ev3, rx, sizeof(rx));
	if(err != EV3_OK)
		return -2;

	#ifdef __DEBUG_EV3_SENSOR
	printf("read:");
	for(unsigned int i = 0; i<sizeof(rx); i++) {
		printf(" %x", rx[i]);
	}
	printf("\n");
	#endif

	return (int)(rx[5]);
} // readSensor


ev3_error_t ev3_USBCom::ev3_init( ev3_t **ev3 )  {
	if(err == EV3_NOT_INITIALIZED) {
		usb_init( );
		*ev3 = (ev3_t*)(calloc( 1, sizeof( **ev3 ) ));
		err = EV3_NOT_PRESENT;
	}

	return err;
} // ev3_init


ev3_error_t ev3_USBCom::ev3_find( ev3_t *ev3 )  {
	if(err == EV3_NOT_PRESENT) {
		struct usb_bus *busses, *bus;

		usb_find_busses( );
		usb_find_devices( );

		busses = usb_get_busses( );

		for ( bus = busses; bus != NULL; bus = bus->next )  {
			struct usb_device *dev;

			for( dev = bus->devices; dev != NULL; dev = dev->next )  {
				if( dev->descriptor.idVendor == VENDOR_LEGO &&
					dev->descriptor.idProduct == PRODUCT_EV3 )  {
						ev3->dev = dev;
						err = EV3_USB_WAIT_TO_CONNECT;
						return err;
				}
			}
		}

		err = EV3_NOT_PRESENT;
	}
	return err;
} // ev3_find


ev3_error_t ev3_USBCom::ev3_open( ev3_t *ev3 )  {
	char bound_driver_name[EV3_USB_NAME_LENGTH];
	int ret;

	ev3->hdl = usb_open( ev3->dev );

	/* Detach possible kernel driver bound to interface */
	if ( usb_get_driver_np( ev3->hdl, EV3_INTERFACE_NUMBER, bound_driver_name, EV3_USB_NAME_LENGTH ) == 0 )  {
		if ( usb_detach_kernel_driver_np( ev3->hdl, EV3_INTERFACE_NUMBER ) < 0 )  {
			err = EV3_IN_USE;
			return err;
		}
	}

	ret = usb_set_configuration( ev3->hdl, EV3_CONFIGURATION_NB );

	if ( ret < 0 )  {
		usb_close( ev3->hdl );
		err = EV3_CONFIGURATION_ERROR;
		return err;
	}

	ret = usb_claim_interface( ev3->hdl, EV3_INTERFACE_NUMBER );

	if ( ret < 0 )  {
		usb_close( ev3->hdl );
		err = EV3_IN_USE;
		return err;
	}
	err = EV3_OK;
	return err;
} // ev3_open


ev3_error_t ev3_USBCom::ev3_close( ev3_t *ev3 )  {

	usb_release_interface( ev3->hdl, EV3_INTERFACE_NUMBER );
	usb_close( ev3->hdl );
	free( ev3 );
	err = EV3_NOT_INITIALIZED;

	return err;
} // ev3_close


ev3_error_t ev3_USBCom::ev3_send_buf( ev3_t *ev3, unsigned char *buf, int len )  {

	int ret = usb_bulk_write( ev3->hdl, EV3_EP_OUT, (char*)buf, len, EV3_USB_TIMEOUT );

	if ( ret < 0 ) {
		err = EV3_USB_WRITE_ERROR;
		return err;
	}
	err = EV3_OK;
	return err;
} // ev3_send_buf


ev3_error_t ev3_USBCom::ev3_recv_buf( ev3_t *ev3, char *buf, int len )  {

	int ret = usb_bulk_read( ev3->hdl, EV3_EP_IN, buf, len, EV3_USB_TIMEOUT );

	if ( ret < 0 ) {
		err = EV3_USB_READ_ERROR;
		return err;
	}

  return EV3_OK;
} // ev3_recv_buf

#endif
