// EMGLogger.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int opiuce_raw(HANDLE *comportptr, OPIPKT_t* pktptr)
{
    pktptr->dataCode = 0x20;
    pktptr->payload[0] = 0x24;
    pktptr->length = 1;
    if (opipkt_put_com(pktptr, comportptr) != 0) return -1;
    if (opipkt_get_com(pktptr, comportptr) != 0) return -1;
    //if (!((pktptr->dataCode == 0x10) && (1))) return -1;
    else return 0;
}

int opiuce_raw(HANDLE *comportptr, OPIPKT_t* pktptr, int dataCode, int payload)
{
    pktptr->dataCode = dataCode;
    pktptr->payload[0] = payload;
    pktptr->length = 1;
    if (opipkt_put_com(pktptr, comportptr) != 0) return -1;
    if (opipkt_get_com(pktptr, comportptr) != 0) return -1;
    //if (!((pktptr->dataCode == 0x10) && (1))) return -1;
    else return 0;
}

typedef long long int qint64;

void LogValue(qint64 adcValue,std::ofstream *ofs)
{
	// Invert and add one: two's compliment.
	//adcValue = (~adcValue)+0x01;
						
	//printf ("0x%.4X\n", adcValue);
	char code[256];

	if ((adcValue < 0x8000))
	{
		sprintf (code,"%I64d",adcValue);

		(*ofs) << code << " ";
	}
	else
	{
		sprintf (code,"-%I64d",(~(adcValue-1))&0x0000FFFF);

		(*ofs) << code << " ";
	}
}

int readdata(HANDLE comPort, std::ofstream *ofs)
{
	OPIPKT_t tseDataOpipkt;
	OPIPKT_t tseFFTOpipkt;
	qint64 prevFrmTS=0;
	qint64 adcValue;
	qint64 sampQual;
	qint64 noPktTimerCt;
	int i;

	// *** Get Data
	// get new data on comPort and stuff into correct buffers (drawing, fft, led, file, posture viewer)
	int getDataRes = opiuce_getwltsedata(&comPort, &tseDataOpipkt);
	if (getDataRes == -1)
	{
		printf ("Error reading data...\n");
		return -1;
	}
	else
		if (getDataRes == 0)
		{
			// Async poolling, data may not be found.  Skip
			//printf ("No data received....\n");
			return 0;
		}
		else
	if(getDataRes == NEWTSEDATACODE)
	{
		noPktTimerCt = 0;   // reset here
		// get first sample and check sample quality to determine if it should be used
		int frmTS = (((qint64) tseDataOpipkt.payload[WFRMHDRLEN-1]) << 40) + (((qint64) tseDataOpipkt.payload[WFRMHDRLEN-1+1]) << 32) +
				(((qint64) tseDataOpipkt.payload[WFRMHDRLEN-1+2]) << 24) + ((tseDataOpipkt.payload[WFRMHDRLEN-1+3]) << 16) +
				(tseDataOpipkt.payload[WFRMHDRLEN-1+4] << 8) + (tseDataOpipkt.payload[WFRMHDRLEN-1+5]);
				
		if(frmTS < prevFrmTS)
		{
			printf("error, packets not monotonically increasing");
			return -1;
		}
		prevFrmTS = frmTS;
		adcValue = ((tseDataOpipkt.payload[1+TSLEN+WLFRMHDRLEN]) << 8) + tseDataOpipkt.payload[1+TSLEN+WLFRMHDRLEN+1];
		sampQual = adcValue & SAMPQUALMASK;
		adcValue = adcValue & ~SAMPQUALMASK;

		if(sampQual <= 3)    // take only data with adequate signal quality 0= CRC=1, CRC=0 1=noC; 2=<20C; 3=>20C;
		{
			// if we took it, then hurry and get UCE calculated FFT data
			if(opiuce_getwltsefft(&comPort, &tseFFTOpipkt))
			{
				printf( "error getting fft");;
				return -1;
			}

			// ADC Signal data.
			LogValue(adcValue,ofs);
			(*ofs) << std::endl;

			if(tseDataOpipkt.length == TSEFRMLEN) // full 64 adc samples here
			{
				for(i = 1; i < ADCLEN; i++) // already got first sample
				{
					adcValue = ((tseDataOpipkt.payload[WFRMHDRLEN-1+TSLEN+WLFRMHDRLEN+2*i]) << 8) + tseDataOpipkt.payload[WFRMHDRLEN-1+TSLEN+WLFRMHDRLEN+2*i+1];
                            							
					LogValue(adcValue,ofs);
					(*ofs) << std::endl;
				}

				qint64 temp = ((tseDataOpipkt.payload[1+TSLEN+WLFRMHDRLEN+2*ADCLEN]) << 8) + tseDataOpipkt.payload[1+TSLEN+WLFRMHDRLEN+2*ADCLEN+1];
				qint64 accelx = ((tseDataOpipkt.payload[1+TSLEN+WLFRMHDRLEN+2*(ADCLEN+1)]) << 8) + tseDataOpipkt.payload[1+TSLEN+WLFRMHDRLEN+2*(ADCLEN+1)+1];
				qint64 accely = ((tseDataOpipkt.payload[1+TSLEN+WLFRMHDRLEN+2*(ADCLEN+2)]) << 8) + tseDataOpipkt.payload[1+TSLEN+WLFRMHDRLEN+2*(ADCLEN+2)+1];
				qint64 accelz = ((tseDataOpipkt.payload[1+TSLEN+WLFRMHDRLEN+2*(ADCLEN+3)]) << 8) + tseDataOpipkt.payload[1+TSLEN+WLFRMHDRLEN+2*(ADCLEN+3)+1];

				//printf ("%I64d",temp);
				//printf ("\n");

			}
			else if(tseDataOpipkt.length == (TSEFRMLEN-4)) // only 62 adc samples
			{
				for(i = 1; i < ADCLEN-2; i++) // already got first sample
				{
					adcValue = ((tseDataOpipkt.payload[WFRMHDRLEN-1+TSLEN+WLFRMHDRLEN+2*i]) << 8) + tseDataOpipkt.payload[WFRMHDRLEN-1+TSLEN+WLFRMHDRLEN+2*i+1];
                            
					LogValue(adcValue,ofs);
					(*ofs) << std::endl;
				}  
				qint64 temp = ((tseDataOpipkt.payload[1+TSLEN+WLFRMHDRLEN+2*ADCLEN-2]) << 8) + tseDataOpipkt.payload[1+TSLEN+WLFRMHDRLEN+2*ADCLEN-2+1];
				qint64 accelx = ((tseDataOpipkt.payload[1+TSLEN+WLFRMHDRLEN+2*(ADCLEN-2+1)]) << 8) + tseDataOpipkt.payload[1+TSLEN+WLFRMHDRLEN+2*(ADCLEN-2+1)+1];
				qint64 accely = ((tseDataOpipkt.payload[1+TSLEN+WLFRMHDRLEN+2*(ADCLEN-2+2)]) << 8) + tseDataOpipkt.payload[1+TSLEN+WLFRMHDRLEN+2*(ADCLEN-2+2)+1];
				qint64 accelz = ((tseDataOpipkt.payload[1+TSLEN+WLFRMHDRLEN+2*(ADCLEN-2+3)]) << 8) + tseDataOpipkt.payload[1+TSLEN+WLFRMHDRLEN+2*(ADCLEN-2+3)+1];

				//printf ("%I64d",temp);
				//printf ("\n");

			}

		}
		else
		{
			printf ("Signal Quality is not good....\n");
			return -1;
		}
	}
	return 0;
}


int LogEmg(HANDLE comPort)
{
	char *fullfilename = new char[255];

	// TODO Add more complex filename creation rules.
	strcpy(fullfilename,"emg.dat");

	printf ("Reading information from sensor into %s\n", fullfilename);

	std::ofstream *ofs;   

	ofs = new std::ofstream(fullfilename,std::ios::trunc);

	int tics = 0;

	while (tics < 512*100 )
	{
		if (readdata(comPort, ofs) == -1)
			break;
		tics++;
	}

	ofs->close();

	delete ofs;

	return 0;
}



int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE comPort;

	OPIPKT_t pktptr;

	if(opi_openuce_com(&comPort))   // open com port, should open unless device was removed
	{
		opi_closeuce_com(&comPort);
		comPort = 0;    
		printf("COM ERROR");
	}
	printf ("Com open at:%d\n", comPort);
	if(comPort != 0)
	{
		if(!opiuce_onmode(&comPort))
		{
			//opiuce_status(HANDLE *comportptr, OPIPKT_t* pktptr);
			printf("OPI UCE is now ON!\n");
		}
	}


	if (!opiuce_status(&comPort, &pktptr))
	{
		printf("Status information retrieved:\n");
		opipkt_dump(&pktptr);
	}
	else
	{
		printf("Error accessing\n");
	}

	if(!opiuce_getrelaxparams(&comPort, &pktptr))
	{
		printf ("Relax Parameters:\n");
		opipkt_dump(&pktptr);
	}

	if (!opiuce_getrelaxdata(&comPort, &pktptr))
	{
		printf ("Relax Data:\n");
		opipkt_dump(&pktptr);
	}

	if (!opiuce_resetrelaxdata(&comPort))
	{
				printf ("Relax Reset:\n");
		opipkt_dump(&pktptr);
	}

	int pktNumber = 0;

	if (opiuce_turnusdspion(&comPort))
	{
		printf ("Error Turning on spi");
	}

	if (!opiuce_get5mmtsdata(&comPort, pktNumber, &pktptr))
	{
		printf ("Get 5 mts data...\n");
		opipkt_dump(&pktptr);
	}
	else
		printf ("No data has been read.\n");


	if (!opiuce_raw(&comPort, &pktptr))
	{
		printf ("Raw Data:");
		opipkt_dump(&pktptr);
	}
	else
	{
		printf("Error Reading raw data...\n");
	}

	if (!opiuce_get5mmtsdata(&comPort, pktNumber, &pktptr))
	{
		printf ("Get 5 mts data...\n");
		opipkt_dump(&pktptr);
	}
	else
		printf ("No data has been read.\n");

	opiuce_settserfmode(&comPort, 0x01);

	if (!opiuce_get5mmtsdata(&comPort, pktNumber, &pktptr))
	{
		printf ("Get 5 mts data...\n");
		opipkt_dump(&pktptr);
	}
	else
		printf ("No data has been read.\n");

	opiuce_turnmodon(&comPort);
	
	//opi_closeuce_com(&comPort);


	// Asegurate que el sensor esté conectado (se enciende la luz naranja cuando està en el USB).  Conecta nuevamente el sensor al controlador, 
	// y fijate que se encienda una roja intermitente (fijate que al apretar los botones - y + se encienden las luces rojas y azules).
	// Saca el sensor y tiene que empezar a titilar la blanca, si no lo hace, conecta nuevamente el sensor y volvelo a sacar.
	// con el sensor fuera del controlador, ejecuta este programa con la parte de configuración comentada.

	// @TODO:  Tomar el codigo de relax que hace la configuración y replicarlo acà)


/**
	if(opi_openuce_com(&comPort))   // open com port, should open unless device was removed
	{
		opi_closeuce_com(&comPort);
		comPort = 0;    
		printf("COM ERROR");
	}
	//printf ("Com open at:%d\n", comPort);



**/

	// Shutdown
	//opiuce_raw(&comPort, &pktptr, 0x13,0x00);

	opiuce_raw(&comPort, &pktptr, 0x20,0x23);

	LogEmg(comPort);

	opi_closeuce_com(&comPort);

	return 0;
}

