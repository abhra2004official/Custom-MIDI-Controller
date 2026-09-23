#include<iostream>
#include<cstdlib>
#include<windows.h>

using namespace std;

class mySerial{

private:
    HANDLE myPort;
    HMIDIOUT myMidi;
    char buffer[100];
    DWORD bytesRead = 0;

public:
    mySerial(const char *portName, int rate, int midiPort){
        myPort = CreateFileA(portName, GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
        if(myPort == INVALID_HANDLE_VALUE){
            DWORD error = GetLastError();
            cout<<"port open failed !" << endl;
            cout<<"error code : "<< error << endl;
            return;
        }

        DCB dcb = { };
        dcb.DCBlength = sizeof(dcb);

        if(!GetCommState(myPort, &dcb)){
            DWORD error = GetLastError();
            cout<<"counldnt fetch info from port"<<endl;
            cout<<"error code : "<< error << endl;
            return;
        }

        dcb.BaudRate = rate;
        dcb.ByteSize = 8;
        dcb.StopBits = ONESTOPBIT;
        dcb.Parity = NOPARITY;

        if(!SetCommState(myPort, &dcb)){
            DWORD error = GetLastError();
            cout << "couldnt config port" << endl;
            cout << "error code : "<< error << endl;
            return;
        }

        cout << "COM port opened and configured !" << endl;

        MMRESULT result = midiOutOpen(&myMidi, midiPort, 0, 0, 0);
        if(result != MMSYSERR_NOERROR){
            DWORD error = GetLastError();
            cout<<"couldnt open midi port "<<endl;
            cout<<"error code : "<<error << endl;
        }
        else{
            cout<<"midi port opened"<<endl;
        }
    }
    

    void read(){

        DWORD newbytes = 0;
        DWORD space_left = sizeof(buffer) - 1 - bytesRead;

        if(space_left == 0){
            cout<<"buffer full with garbage " << "resetting buffer!" << endl;
            bytesRead = 0;
            return;
        }

        if(ReadFile(myPort, buffer + bytesRead, space_left, &newbytes, NULL)){
            bytesRead = bytesRead + newbytes;
            buffer[bytesRead] = '\0';
        }
        else{
            DWORD error = GetLastError();
            cout<<"read failed!"<<endl;
            cout<<"error code : "<< error <<endl;
            return;
        }

        while(true){

            int end = 0;
            bool found = false;

            for(int i = 0; i < (int)bytesRead; i++){
                if(buffer[i] == '\n'){
                    end = i;
                    found = true;
                    break;
                }
            }

            if(!found) break;

            int value = atoi(buffer);

            DWORD msg = 0xB0 | 1 << 8 | value << 16;
            midiOutShortMsg(myMidi, msg);

            int leftoverStart = end + 1;
            int leftoverBytes = bytesRead - leftoverStart;

            if(leftoverBytes > 0){
                memmove(buffer, buffer + leftoverStart, leftoverBytes);
            }

            bytesRead = leftoverBytes;
            buffer[leftoverBytes] = '\0';
        }

        return;
    }

    ~mySerial(){
        CloseHandle(myPort);
        midiOutClose(myMidi);
        cout<<"ports closed !!" << endl;
    }
};

int main(){

    int devices = midiOutGetNumDevs();
    MIDIOUTCAPSA devs;

    for(int i = 0; i < devices; i++){

        midiOutGetDevCapsA(i, &devs, sizeof(devs));
        cout<< i << " -> "<< devs.szPname<<endl;
    }

    int midiPort = 0;
    cout<<"\nchoose midi port : ";
    cin>> midiPort;

    mySerial port("\\\\.\\COM13", 9600, midiPort);

    while(true) port.read();
    
    return 0;
}
