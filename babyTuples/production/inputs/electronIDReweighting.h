#ifndef ELECTRONID_REWEIGHTING
#define ELECTRONID_REWEIGHTING

pair<float,float> muonIDEfficiency(float pT, float eta)
{
    if (pT < 20) return pair<float,float>(0.0,0.0);
    if (fabs(eta) > 2.1) return pair<float,float>(0.0,0.0);

    if (fabs(eta) < 0.8)
    {
             if (pT < 30)  return pair<float,float>(0.9839,0.0006);
        else if (pT < 40)  return pair<float,float>(0.9850,0.0003);
        else if (pT < 50)  return pair<float,float>(0.9865,0.0003);
        else if (pT < 60)  return pair<float,float>(0.9829,0.0006);
        else if (pT < 80)  return pair<float,float>(0.9835,0.0012);
        else if (pT < 100) return pair<float,float>(0.9785,0.0031);
        else if (pT < 150) return pair<float,float>(0.9847,0.0042);
        else if (pT < 200) return pair<float,float>(0.9958,0.0101);
        else if (pT < 300) return pair<float,float>(0.9937,0.0215);
        else               return pair<float,float>(0.9754,0.0663);
    }
    else if (fabs(eta) < 1.5)
    {
             if (pT < 30)  return pair<float,float>(0.9850,0.0008);
        else if (pT < 40)  return pair<float,float>(0.9846,0.0004);
        else if (pT < 50)  return pair<float,float>(0.9866,0.0003);
        else if (pT < 60)  return pair<float,float>(0.9834,0.0007);
        else if (pT < 80)  return pair<float,float>(0.9818,0.0015);
        else if (pT < 100) return pair<float,float>(0.9803,0.0039);
        else if (pT < 150) return pair<float,float>(0.9765,0.0054);
        else if (pT < 200) return pair<float,float>(1.0064,0.0145);
        else if (pT < 300) return pair<float,float>(0.9867,0.0339);
        else               return pair<float,float>(1.0348,0.1693);
    }
    else if (fabs(eta) < 2.1)
    {
             if (pT < 30)  return pair<float,float>(0.9876,0.0010);
        else if (pT < 40)  return pair<float,float>(0.9890,0.0006);
        else if (pT < 50)  return pair<float,float>(0.9902,0.0005);
        else if (pT < 60)  return pair<float,float>(0.9864,0.0012);
        else if (pT < 80)  return pair<float,float>(0.9909,0.0024);
        else if (pT < 100) return pair<float,float>(0.9995,0.0070);
        else if (pT < 150) return pair<float,float>(0.9884,0.0102);
        else if (pT < 200) return pair<float,float>(0.9613,0.0279);
        else if (pT < 300) return pair<float,float>(0.9652,0.0720);
        else               return pair<float,float>(0.4286,0.4676);
    }
    return pair<float,float>(0.0,0.0);
}

pair<float,float> muonIsoScaleFactor(float pT, float eta)
{
    if (pT < 20) return pair<float,float>(0.0,0.0);
    if (fabs(eta) > 2.1) return pair<float,float>(0.0,0.0);

    if (fabs(eta) < 0.8)
    {
             if (pT < 30)  return pair<float,float>(0.9934, 0.0010);
        else if (pT < 40)  return pair<float,float>(0.9969, 0.0003);
        else if (pT < 50)  return pair<float,float>(0.9979, 0.0002);
        else if (pT < 60)  return pair<float,float>(0.9985, 0.0005);
        else if (pT < 80)  return pair<float,float>(0.9989, 0.0011);
        else if (pT < 100) return pair<float,float>(0.9999, 0.0031);
        else if (pT < 150) return pair<float,float>(1.0014, 0.0043);
        else if (pT < 200) return pair<float,float>(0.9802, 0.0109);
        else if (pT < 300) return pair<float,float>(1.0016, 0.0171);
        else               return pair<float,float>(0.9923, 0.0377);
    }
    else if (fabs(eta) < 1.5)
    {
             if (pT < 30)  return pair<float,float>(0.9974, 0.0011);
        else if (pT < 40)  return pair<float,float>(1.0004, 0.0004);
        else if (pT < 50)  return pair<float,float>(1.0001, 0.0002);
        else if (pT < 60)  return pair<float,float>(1.0007, 0.0005);
        else if (pT < 80)  return pair<float,float>(0.9997, 0.0011);
        else if (pT < 100) return pair<float,float>(1.0075, 0.0034);
        else if (pT < 150) return pair<float,float>(1.0056, 0.0049);
        else if (pT < 200) return pair<float,float>(1.0203, 0.0139);
        else if (pT < 300) return pair<float,float>(1.0059, 0.0200);
        else               return pair<float,float>(0.9822, 0.0681);
    }
    else if (fabs(eta) < 2.1)
    {
             if (pT < 30)  return pair<float,float>(1.0068, 0.0011);
        else if (pT < 40)  return pair<float,float>(1.0039, 0.0004);
        else if (pT < 50)  return pair<float,float>(1.0023, 0.0003);
        else if (pT < 60)  return pair<float,float>(1.0042, 0.0006);
        else if (pT < 80)  return pair<float,float>(1.0046, 0.0013);
        else if (pT < 100) return pair<float,float>(1.0086, 0.0042);
        else if (pT < 150) return pair<float,float>(1.0071, 0.0053);
        else if (pT < 200) return pair<float,float>(0.9582, 0.0129);
        else if (pT < 300) return pair<float,float>(1.0261, 0.0398);
        else               return pair<float,float>(1.0000, 0.0000);
    }
    return pair<float,float>(0.0,0.0);
}

pair<float,float> elecIDEfficiency(float pT, float eta)
{
    if (pT < 20) return pair<float,float>(0.0,0.0);
    if (fabs(eta) > 1.4442) return pair<float,float>(0.0,0.0);

    if (fabs(eta) < 0.8)
    {
             if (pT < 30)  return pair<float,float>(0.9923,0.0022);
        else if (pT < 40)  return pair<float,float>(0.9883,0.0008);
        else if (pT < 50)  return pair<float,float>(0.9900,0.0006);
        else if (pT < 60)  return pair<float,float>(0.9880,0.0012);
        else if (pT < 80)  return pair<float,float>(0.9847,0.0024);
        else if (pT < 100) return pair<float,float>(0.9924,0.0062);
        else if (pT < 150) return pair<float,float>(0.9892,0.0081);
        else if (pT < 200) return pair<float,float>(1.0216,0.0191);
        else if (pT < 300) return pair<float,float>(0.9869,0.0320);
        else               return pair<float,float>(1.0789,0.0854);
    }
    else if (fabs(eta) < 1.4442)
    {
             if (pT < 30)  return pair<float,float>(0.9632,0.0022);
        else if (pT < 40)  return pair<float,float>(0.9707,0.0010);
        else if (pT < 50)  return pair<float,float>(0.9755,0.0008);
        else if (pT < 60)  return pair<float,float>(0.9777,0.0017);
        else if (pT < 80)  return pair<float,float>(0.9797,0.0032);
        else if (pT < 100) return pair<float,float>(0.9687,0.0081);
        else if (pT < 150) return pair<float,float>(0.9813,0.0110);
        else if (pT < 200) return pair<float,float>(0.9940,0.0286);
        else if (pT < 300) return pair<float,float>(0.8853,0.0408);
        else               return pair<float,float>(1.0286,0.1733);
    }
    return pair<float,float>(0.0,0.0);
}

pair<float,float> elecIsoScaleFactor(float pT, float eta)
{
    if (pT < 20) return pair<float,float>(0.0,0.0);
    if (fabs(eta) > 1.4442) return pair<float,float>(0.0,0.0);

    if (fabs(eta) < 0.8)
    {
             if (pT < 30)  return pair<float,float>(0.9938,0.0015);
        else if (pT < 40)  return pair<float,float>(0.9968,0.0004);
        else if (pT < 50)  return pair<float,float>(0.9973,0.0002);
        else if (pT < 60)  return pair<float,float>(0.9957,0.0005);
        else if (pT < 80)  return pair<float,float>(0.9962,0.0012);
        else if (pT < 100) return pair<float,float>(0.9992,0.0035);
        else if (pT < 150) return pair<float,float>(0.9964,0.0052);
        else if (pT < 200) return pair<float,float>(0.9861,0.0117);
        else if (pT < 300) return pair<float,float>(1.0025,0.0256);
        else               return pair<float,float>(1.1525,0.0944);
    }                                                      
    else if (fabs(eta) < 1.5)                              
    {                                                      
             if (pT < 30)  return pair<float,float>(0.9939,0.0012);
        else if (pT < 40)  return pair<float,float>(0.9963,0.0005);
        else if (pT < 50)  return pair<float,float>(0.9965,0.0003);
        else if (pT < 60)  return pair<float,float>(0.9963,0.0008);
        else if (pT < 80)  return pair<float,float>(0.9952,0.0017);
        else if (pT < 100) return pair<float,float>(1.0013,0.0055);
        else if (pT < 150) return pair<float,float>(0.9882,0.0077);
        else if (pT < 200) return pair<float,float>(1.0068,0.0196);
        else if (pT < 300) return pair<float,float>(1.0076,0.0344);
        else               return pair<float,float>(1.0084,0.0926);
    }
    return pair<float,float>(0.0,0.0);
}

// #################################################################

pair<float,float> leptonIDEfficiency(int pdgId, float pT, float eta)   
{  
         if (abs(pdgId) == 11) return elecIDEfficiency(pT,eta);
    else if (abs(pdgId) == 13) return muonIDEfficiency(pT,eta);
    else                       return pair<float,float>(-1.0,-1.0);
}
pair<float,float> leptonIsoScaleFactor(int pdgId, float pT, float eta)
{
         if (abs(pdgId) == 11) return elecIsoScaleFactor(pT,eta);
    else if (abs(pdgId) == 13) return muonIsoScaleFactor(pT,eta);
    else                       return pair<float,float>(-1.0,-1.0);
}


#endif
