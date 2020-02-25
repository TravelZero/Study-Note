#include "uart.h"

int UartInitDev(char *pDev, int nSpeed, int databits, int stopbits, int parity)
{
	if (NULL == pDev) return -1;

    int filed = 0;

	if((filed = open(pDev, O_RDWR|O_NOCTTY, 0777)) < 0)
    {
        fprintf(stderr, "open %s failed\n", pDev);
    }
    else
    {
		UartSetSpeed(filed, nSpeed);
	    UartSetParity(filed, databits, stopbits, parity);
    }
    	
    return filed;
}

int UartCloseDev(int fd)
{
    return close(fd);
}

int UartSetSpeed(int fd, int nSpeed)
{
	int i = 0;
    int speed[] = {B921600, B115200, B57600, B38400, B19200, B9600, B4800, B2400, B1200, B300, };
    int intSpeed[] = {921600, 115200, 57600, 38400, 19200, 9600, 4800, 2400, 1200, 300, };
    struct termios Opt; //定义termios结构

	if(tcgetattr(fd, &Opt) != 0)
	{
		perror("tcgetattr fd");
		return -1;
	}
    
	for(i = 0; i < sizeof(speed) / sizeof(int) / 2; i++)
	{
		if (nSpeed == intSpeed[i])
		{
			tcflush(fd, TCIOFLUSH);
			cfsetispeed(&Opt, speed[i]);
			cfsetospeed(&Opt, speed[i]);

			if (tcsetattr(fd, TCSANOW, &Opt) != 0)
			{
				perror("tcsetattr fd");
				return -1;
			}

			tcflush(fd, TCIOFLUSH);
		}
	}

    return 0;
}

int UartSetParity(int fd, int databits, int stopbits, int parity)
{
	struct termios Opt;
	if (tcgetattr(fd, &Opt) != 0)
	{
		perror("tcgetattr fd");
		return -1;
	}
	Opt.c_cflag |= (CLOCAL | CREAD); //一般必设置的标志

	switch(databits) //设置数据位数
	{
		case 7:
		Opt.c_cflag &= ~CSIZE;
		Opt.c_cflag |= CS7;
		break;
		case 8:
		Opt.c_cflag &= ~CSIZE;
		Opt.c_cflag |= CS8;
		break;
		default:
		fprintf(stderr, "Unsupported data size.\n");
		return -1;
	}

	switch(parity) //设置校验位
	{
		case 'n':
		case 'N':
		Opt.c_cflag &= ~PARENB; //清除校验位
		Opt.c_iflag &= ~INPCK; //enable parity checking
		break;
		case 'o':
		case 'O':
		Opt.c_cflag |= PARENB; //enable parity
		Opt.c_cflag |= PARODD; //奇校验
		Opt.c_iflag |= INPCK;  //disable parity checking
		break;
		case 'e':
		case 'E':
		Opt.c_cflag |= PARENB; //enable parity
		Opt.c_cflag &= ~PARODD; //偶校验
		Opt.c_iflag |= INPCK; //disable pairty checking
		break;
		case 's':
		case 'S':
		Opt.c_cflag &= ~PARENB; //清除校验位
		Opt.c_cflag &= ~CSTOPB; 
		Opt.c_iflag |= INPCK; //disable pairty checking
		break;
		default:
		fprintf(stderr, "Unsupported parity.\n");
		return -1;
	}

	switch(stopbits) //设置停止位
	{
		case 1:
		Opt.c_cflag &= ~CSTOPB;
		break;
		case 2:
		Opt.c_cflag |= CSTOPB;
		break;
		default:
		fprintf(stderr, "Unsupported stopbits.\n");
		return -1;
	}

	Opt.c_cflag |= (CLOCAL | CREAD);

	Opt.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

	Opt.c_oflag &= ~OPOST;
	Opt.c_oflag &= ~(ONLCR | OCRNL); //添加的

	Opt.c_iflag &= ~(ICRNL | INLCR);
	Opt.c_iflag &= ~(IXON | IXOFF | IXANY); //添加的

	tcflush(fd, TCIFLUSH);
	Opt.c_cc[VTIME] = 0; //设置超时为15sec
	Opt.c_cc[VMIN] = 0; //Update the Opt and do it now
	if (tcsetattr(fd, TCSANOW, &Opt) != 0)
	{
		perror("tcsetattr fd");
		return -1;
	}

	return 0;
}

ssize_t UartSendData(int fd, char *pData, size_t len)
{
	if (NULL == pData  || len == 0) return -1;

	ssize_t ByteLen = 0;

	ByteLen = write(fd, pData, len);

	return ByteLen;
}	

ssize_t UartRecData(int fd, char *pData, size_t len)
{
	if (NULL == pData  || len == 0) return -1;

	ssize_t ByteLen = 0;
	unsigned char i = 0;

	ByteLen = read(fd, pData, len);

	return ByteLen;
}