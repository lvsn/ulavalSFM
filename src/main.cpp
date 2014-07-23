/*
*	File : main.cpp
*	Author : Émile Robitaille @ LERobot
*	Creation date : 2014, June 27th
*	Version : 1.0
*	
*	Description : Main of ulavalSFM
*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <iostream>

#include "util.h"
#include "dosift.h"
#include "domatch.h"
#include "dogeometry.h"

using namespace std;

int main(int argc, char* argv[])
{
	Opts o;

	//parsing arguments
	parseArgs(argc, argv, o);

	switch (o.choice)
	{
		case 0:
		printError();
		break;
		case 1:
		printHelp();
		break;
		case 2:
		printVersion();
		break;
		case 4:
		listDir(o.dir);
		break;
		case 8:
		cout << endl;
		cout << "Process(es) will do sift with " << o.cores << " core(s) on the " << BTOS(o.cluster) << ", working on \"" << o.dir.getPath() << "\" images." << endl << endl;
		if (o.cores > 1)
		{
			if (o.cluster)
			{
				cout << "MPI - ON SUPERCOMPTER" << endl << endl;
			}
			else
			{
				o.dir.printInfo();
				siftMCore(o.dir.getPath(), o.cores);
				o.dir.update();
				o.dir.printInfo();
			}
		}
		else
		{
			if (o.cluster)
			{
				cout << "NO MPI - ON SUPERCOMPTER" << endl << endl;
			}
			else
			{
				o.dir.printInfo();
				sift1Core(o.dir);
				o.dir.update();
				o.dir.printInfo();
			}
		}
		break;
		case 16:
		cout << endl;
		cout << "Process(es) will do matches with " << o.cores << " core(s) on the " << BTOS(o.cluster) << ", working on \"" << o.dir.getPath() << "\" images." << endl << endl;
		if (o.cores > 1)
		{
			if (o.cluster)
			{
				cout << "MPI - ON SUPERCOMPTER" << endl << endl;
			}
			else
			{
				o.dir.printInfo();
				matchMCore(o.dir.getPath(), o.cores);
				o.dir.update();
				o.dir.printInfo();
			}
		}
		else
		{
			if (o.cluster)
			{
				cout << "NO MPI - ON SUPERCOMPTER" << endl << endl;
			}
			else
			{
				o.dir.printInfo();
				match1Core(o.dir);
				o.dir.update();
				o.dir.printInfo();
			}
		}
		break;
		case 32:
		cout << endl;
		cout << "Process(es) will compute geometric constraints with " << o.cores << " core(s) on the " << BTOS(o.cluster) << ", working on \"" << o.dir.getPath() << "\" images." << endl << endl;
		if (o.cores > 1)
		{
			if (o.cluster)
			{
				cout << "MPI - ON SUPERCOMPTER" << endl << endl;
			}
			else
			{
				o.dir.printInfo();
				geometryMCore(o.dir.getPath(), o.cores);
				o.dir.update();
				o.dir.printInfo();
			}
		}
		else
		{
			if (o.cluster)
			{
				cout << "NO MPI - ON SUPERCOMPTER" << endl << endl;
			}
			else
			{
				o.dir.printInfo();
				geometry1Core(o.dir);
				o.dir.update();
				o.dir.printInfo();
			}
		}
		break;
		case 64:
		cout << endl;
		cout << "Process(es) will do sift, matches and geomtry with " << o.cores << " core(s) on the " << BTOS(o.cluster) << ", working on \"" << o.dir.getPath() << "\" images." << endl << endl;
		if (o.cores > 1)
		{
			if (o.cluster)
			{
				cout << "MPI - ON SUPERCOMPTER" << endl << endl;
			}
			else
			{
				o.dir.printInfo();
				siftMCore(o.dir.getPath(), o.cores);
				matchMCore(o.dir.getPath(), o.cores);
				geometryMCore(o.dir.getPath(), o.cores);
				o.dir.update();
				o.dir.printInfo();
			}
		}
		else
		{
			if (o.cluster)
			{
				cout << "NO MPI - ON SUPERCOMPTER" << endl << endl;
			}
			else
			{
				o.dir.printInfo();
				sift1Core(o.dir);
				o.dir.update();
				match1Core(o.dir);
				o.dir.update();
				geometry1Core(o.dir);
				o.dir.update();
				o.dir.printInfo();
			}
		}
		break;
		default:
		printError();
		break;
	}
}
