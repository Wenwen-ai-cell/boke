#include <iostream>
#include <fstream>
#include <okFrontPanel.h>
#include <okFrontPanelDLL.h>
using namespace std;

int i;
int flag;
int main()
{
	
	okCFrontPanel dev;
	okCFrontPanel::ErrorCode error;
	unsigned char dataout[2];
	unsigned char datain[128];
	int i;
	long written;

	dev.OpenBySerial();
	error = dev.ConfigureFPGA("Counters.bit");
	ofstream outFile("data.txt");
	// It’s a good idea to check for errors here!

	// Send brief reset signal to initialize the FIFO.
	//dev.SetWireInValue(0x10, 0xff, 0x01);
	//dev.UpdateWireIns();
	//dev.SetWireInValue(0x10, 0x00, 0x11);
	//dev.UpdateWireIns();

	dev.SetWireInValue(0x10, 0xff);
	dev.UpdateWireIns();
	dev.SetWireInValue(0x10, 0x00);
	dev.UpdateWireIns();


	for (i = 1;i>=0; i--) {
		// Load outgoing data buffer.
		dataout[i]=i;
	}
	dev.UpdateWireIns();
	// Send buffer to PipeIn endpoint with address 0x80
	//written = dev.WriteToPipeIn(0x10, sizeof(dataout),dataout);
	// Read to buffer from PipeOut endpoint with address 0xA0
	written = dev.ReadFromPipeOut(0xA0, sizeof(datain), datain);
	
	if (!outFile)
	{
		cout << "无法打开文件" << endl;
	}
	//for (i = 0; i < sizeof(datain); i++) {
	//	cout << static_cast<int>(datain[i]) << " "; // Print each element of datain
	//	outFile << static_cast<int>(datain[i]) << endl;
	//}
	for (i = 0; i <sizeof(datain); i++) {
		//cout << static_cast<int>(datain[i]) << " "; // Print each element of datain
		cout << (int)(datain[i]) << " ";
		outFile << static_cast<int>(datain[i]) << endl;
	}

}