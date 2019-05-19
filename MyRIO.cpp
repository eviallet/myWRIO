#include "MyRIO.h"

using namespace myRIO;

NiFpga_Status status = 0; /**< Initialize the status to 0 */

bool myRIO::myRIO_init() {
	status = MyRio_Open();
	return MyRio_IsNotSuccess(status)?false:true;
}

bool myRIO::myRIO_close() {
	status = MyRio_Close();
	return MyRio_IsNotSuccess(status)?false:true;
}

/*
 * Return true when there is an error
 */
bool myRIO::myRIO_error() {
	return NiFpga_IsError(status);
}

