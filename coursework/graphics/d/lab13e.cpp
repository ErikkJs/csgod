//Erik Soto
//CMPS 3350 Lab 13
//Lab Challenge 5

#include <iostream>
#include <locale>
#include <cstring>
#include <vector>
#include <algorithm>
using namespace std;



int conv(char str[],int a[][1])
{
    int i =0;
    char temp;

    while(str[i])
    {
        temp=str[i];
        str[i]=(char) toupper((temp));
        temp=str[i];
        a[temp-65][0]++;

        i++;
    }
    return i;
}

void toString(int s[][1],int size)
{
    int i =0;
    while(i<size)
    {
        cout << s[i][0]<<endl;
        i++;
    }


}



void init(int a[][1])
{
    for(int i =0; i<26; i++)
    {
        a[i][0]=0;
    }

}




int main()
{
    char str [] = "dbsCMuCrNKosulKEJEUGUMfssuoQslUGLKAbUubrrESTeCSdcaGlCAboJeETuldcBGCoasTfClMQVeoTGdMEQeCWeooGuBJcSaUTJECdsssuTCeGWleGcoSCMsEUACPEeduhSlfGsrAGaVfAMMUBZUrGSsusbeSSCslRCSorMoSJfKCrreLCSfUSCUaCGeuRsdrbdooGGeeJrXeRooWMcurMuGbUQcalCkoNPABSlAoaLlUCdsTBSCcrMLCBKYKMACSAeLfsosuldUSGKuGWglouCAEruuJWaXGSECGoCSgiobaTCrahEBBdrlUbLcefMfdTlMSNBuKMXYlVArBBoEMdoRrGSZlSTuTgulMMMMgBXGLBuusdJgaToochcCSSKgYGUSAGoeGesMuTaBdBCuNsSogECGgsArruGeCueCefYTGPUAeCSAKAsaUblSRASTJilSlugTWSuluUTWMTeuSeBENKETslRCdTEUleerVcorAQGrZsuuGfuETSdeMrBXSAoeAgUSTVTshSEUlrSaUUXrVruAaUSUASCEUeBTAMeSVehCJuUPKTCNfESBsUlrlSlLBrJeTAoNguEGUCANfSPBZSUbsUJlcfuuBgbrCcBNauEuurSrEBaSrhorBfBldCBEeTUEeGAeoobUUQKrJEBEXUseMWSeshCaEbTrSoCalousNureiPuABVMBerlGBsGSTMTobhdEEBCsEQBfLArbhPBiJcCBBUirKGcsNaGCeTMlfrdlSNTSLPSaCPlUMlshRMeuaUdGKLaSQJoQGBcUlMdufMCuoABJfSUoQMPUeSrGCGABsfJlCUoUsEWgSUgEcLUucUrefBEilKKsPCEoATSErAaaCellaoTTNoJoCgBTegVTMUBoABLBllbEuLLflLCUeRSMocsJrSUULBVesBlBTGEYrJeBruueSiKusUGlAdGbSbUlsilGTUESdoGKMKANAUbNCcUPcMbubSrrBSUoTLshGJMJfCeUBMRBrBUsBBKcNblsCrluflRrcsrsCQQPcddlbrCiRAGCReCabVuAEMeAoMisllWEBTKNrlbsGrEUfQLQBQTEdllsCRUaBuGGXrMGCeoSEfgaksaBuocANsLoGYbAAuUGCrorTlelACCECrSaTAETCPEsBscUUAePTeiJCuBfTUSABBRcRYCZMTTbdrEbfuQXcGTPYr";
    int a[26][1];
    init(a);
    int count=conv(str,a);

    //toString(a,26); 

    int e=26;

    int g=a[0][0];
    int pos =0;
cout << "3350 Lab-13 Challenge #5" << endl;
cout << "by CS GOD" << endl;
cout << endl << "running... " << endl;
cout << "complete." << endl;
cout << "Letters ordered by most frequently occuring is below" << endl;
    for(int i=0; i < e; i++){

        for(int j = 0; j < e; j++)
        {

            if(a[pos][0] < a[j][0])
            {

                pos=j;
                g=a[j][0];
            }

        }

        for(int k = 0; k < a[pos][0]; k++)
        {

            cout << (char)(pos+65);

        }
        a[pos][0]=0;
    }
}


