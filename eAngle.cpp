#include "eAngle.h"

ClassImp(eAngle);

eAngle::eAngle()
{
	create_input_tree(NULL);
	create_output_tree(NULL);
}

eAngle::eAngle(TTree *inT, TTree *outT, TString inFile, int geoNo)
{
	create_input_tree(inT);
	create_output_tree(outT);
/*	
	outTree=outT;
	
	LV_6He = new TLorentzVector();
	LV_1H = new TLorentzVector();
	LV_2H = new TLorentzVector();

	LV_tar_1H = new TLorentzVector();
	LV_tar_2H = new TLorentzVector();
	LV_beam = new TLorentzVector();
*/	maynard = new TOOL();
	s_curFile = inFile;

	vBeam = new TVector3(0.0,0.0,0.0);
	vTarget = new TVector3(0.0, 0.0, 0.0);

	int CD2_Nel=2;
	CD2_A[0]=2;
	CD2_Z[0]=1;
	CD2_W[0]=2;

	CD2_A[1]=12;
	CD2_Z[1]=6;
	CD2_W[1]=1;

	int BoPET_Nel=3;
	BoPET_A[0]=1;
	BoPET_Z[0]=1;
	BoPET_W[0]=8;

	BoPET_A[1]=12;
	BoPET_Z[1]=6;
	BoPET_W[1]=10;

	BoPET_A[2]=16;
	BoPET_Z[2]=8;
	BoPET_W[2]=4;

	h2_Si 		= new ELC(2, 1, si_Nel, 2.35, si_A, si_Z, si_W, 100.,1500);
	h2_BoPET	= new ELC(2, 1, BoPET_Nel, 1.4, BoPET_A, BoPET_Z, BoPET_W, 100.,1500);
	h2_CD2		= new ELC(2, 1, CD2_Nel, 1.0, CD2_A, CD2_Z, CD2_W, 100.,1500);

	h1_Si 		= new ELC(1, 1, si_Nel, 2.35, si_A, si_Z, si_W, 100.,1500);
	h1_BoPET	= new ELC(1, 1, BoPET_Nel, 1.4, BoPET_A, BoPET_Z, BoPET_W, 100.,1500);
	h1_CD2		= new ELC(1, 1, CD2_Nel, 1.0, CD2_A, CD2_Z, CD2_W, 100.,1500);

	he6_Si 		= new ELC(6, 2, si_Nel, 2.35, si_A, si_Z, si_W, 300.,1500);
	he6_CD2		= new ELC(6, 2, CD2_Nel, 1.0, CD2_A, CD2_Z, CD2_W, 200.,1500);
	he6_BoPET	= new ELC(6, 2, BoPET_Nel, 1.4, BoPET_A, BoPET_Z, BoPET_W, 200.,1500);


	//Change all the graphical cuts geometry from number to variable 
	gcut_he6 = new TCutG("He6",maynard->gcut_noPoints(s_curFile, "he6.dat", geoNo), &in_tof, &in_aF5);
	maynard->gcuts_loader(s_curFile, gcut_he6, "he6.dat", geoNo);

	gcut_dehe4 = new TCutG("dehe4", maynard->gcut_noPoints(s_curFile, "dehe4.dat", geoNo),
							&sqretot, &sqrde);
	maynard->gcuts_loader(s_curFile, gcut_dehe4, "dehe4.dat", geoNo);

	gcut_dehe6 = new TCutG("dehe6", maynard->gcut_noPoints(s_curFile, "dehe6.dat", geoNo),
							&sqretot, &sqrde);
	maynard->gcuts_loader(s_curFile, gcut_dehe6, "dehe6.dat", geoNo);

	gcut_AngAng1H_w6He = new TCutG("gcut_AngAng1H_w6He", maynard->gcut_noPoints(s_curFile,
									 "AngAng1H_w6He.dat", geoNo), &sqrang, &sqlang);
	maynard->gcuts_loader(s_curFile, gcut_AngAng1H_w6He, "AngAng1H_w6He.dat", geoNo);

	gcut_AngAng2H_w6He = new TCutG("gcut_AngAng2H_w6He", maynard->gcut_noPoints(s_curFile,
									 "AngAng2H_w6He.dat", geoNo), &sqrang, &sqlang);
	maynard->gcuts_loader(s_curFile, gcut_AngAng2H_w6He, "AngAng2H_w6He.dat", geoNo);

	gcut_AngAng2H_w6He = new TCutG("gcut_AngAng2H_w6He", maynard->gcut_noPoints(s_curFile,
									 "AngAng2H_w6He.dat", geoNo), &sqrang, &sqlang);
	maynard->gcuts_loader(s_curFile, gcut_AngAng2H_w6He, "AngAng2H_w6He.dat", geoNo);

	proKine = new TCutG("proKine",maynard->gcut_noPoints(s_curFile, "proKine.dat", geoNo), &sqldEE, &sqlang);
	maynard->gcuts_loader(s_curFile, proKine, "proKine.dat", geoNo);

	deuKine = new TCutG("deuKine",maynard->gcut_noPoints(s_curFile, "deuKine.dat", geoNo), &sqldEE, &sqlang);
	maynard->gcuts_loader(s_curFile, deuKine, "deuKine.dat", geoNo);

	dEAnglePro = new TCutG("dEAnglePro",maynard->gcut_noPoints(s_curFile, "dEAnglePro.dat", geoNo), &sqlang, &sqlde);
	maynard->gcuts_loader(s_curFile, dEAnglePro, "dEAnglePro.dat", geoNo);

	dEAngleDeu = new TCutG("dEAngleDeu",maynard->gcut_noPoints(s_curFile, "dEAngleDeu.dat", geoNo), &sqlang, &sqlde);
	maynard->gcuts_loader(s_curFile, dEAngleDeu, "dEAngleDeu.dat", geoNo);

	rnd = new TRandom3();

	ToFMWPC = 520.0;
	gasTargetSi = 21.0;

}

eAngle::~eAngle()
{
	delete h2_Si;
	delete h2_CsI;
	delete h2_BoPET;
	delete h2_CD2;

	delete he6_Si;
	delete he6_CsI;
	delete he6_BoPET;
	delete he6_CD2;

	delete maynard;

	delete rnd;
}

bool eAngle::create_input_tree(TTree *in)
{
	inTree=in;
	inTree->SetMakeClass(1);
	//CALIBRATED DATA
	inTree->SetBranchAddress("CsI_L",	CsI_L);
	inTree->SetBranchAddress("CsI_0L",	CsI_0L);
	inTree->SetBranchAddress("CsI_R",	CsI_R);
	inTree->SetBranchAddress("SQX_L",	SQX_L);
	inTree->SetBranchAddress("SQX_Ln",	SQX_Ln);
	inTree->SetBranchAddress("SQX_R",	SQX_R);
	inTree->SetBranchAddress("SQY_L",	SQY_L);
	inTree->SetBranchAddress("SQY_R",	SQY_R);
	
	//RAW DATA
	inTree->SetBranchAddress("r_CsI_L",	r_CsI_L);
	inTree->SetBranchAddress("r_CsI_R",	r_CsI_R);
	inTree->SetBranchAddress("r_SQX_L",	r_SQX_L);
	inTree->SetBranchAddress("r_SQX_R",	r_SQX_R);
	inTree->SetBranchAddress("r_SQY_L",	r_SQY_L);


	//inTree->SetBranchAddress("SQ300",	SQ300);
	//inTree->SetBranchAddress("r_SQY_R",	r_SQY_R);
	//inTree->SetBranchAddress("r_SQ300",	r_SQ300);
	//inTree->SetBranchAddress("tSQ300",	in_tSQ300);

	//TIME
	inTree->SetBranchAddress("tSQX_L",	in_tSQX_L);
	inTree->SetBranchAddress("tSQX_R",	in_tSQX_R);
	inTree->SetBranchAddress("tCsI_L",	in_tCsI_L);
	inTree->SetBranchAddress("tCsI_R",	in_tCsI_R);
	inTree->SetBranchAddress("tSQY_L",	in_tSQY_L);
	inTree->SetBranchAddress("tSQY_R",	in_tSQY_R);
	

	inTree->SetBranchAddress("tF3",		in_tF3);
	inTree->SetBranchAddress("F3",		in_F3);
	inTree->SetBranchAddress("tF5",		in_tF5);
	inTree->SetBranchAddress("F5",		in_F5);
	inTree->SetBranchAddress("tF6",		in_tF6);
	inTree->SetBranchAddress("F6",		in_F6);

	inTree->SetBranchAddress("nx1",		&in_nx1);
	inTree->SetBranchAddress("ny1",		&in_ny1);
	inTree->SetBranchAddress("nx2",		&in_nx2);
	inTree->SetBranchAddress("ny2",		&in_ny2);

	inTree->SetBranchAddress("tMWPC",	in_tMWPC);

	inTree->SetBranchAddress("x1",		in_x1);
	inTree->SetBranchAddress("y1",		in_y1);
	inTree->SetBranchAddress("x2",		in_x2);
	inTree->SetBranchAddress("y2",		in_y2);

	inTree->SetBranchAddress("trig",	&in_trig);
	inTree->SetBranchAddress("tof",		&in_tof);
	inTree->SetBranchAddress("az",		&in_AZ);
	inTree->SetBranchAddress("aF5",		&in_aF5);
	inTree->SetBranchAddress("run",		&run);


 	return true;
}

bool eAngle::create_output_tree(TTree *out)
{
	outTree=out;
	
	LV_6He = new TLorentzVector();
	LV_1H = new TLorentzVector();
	LV_2H = new TLorentzVector();

	LV_beam = new TLorentzVector();
	outTree->SetMakeClass(1);

	bool longV = true;

	if (longV)
	{
		outTree->Branch("SQX_L_m",	&SQX_L_mult,	"SQX_L_m/S");
		outTree->Branch("SQX_R_m",	&SQX_R_mult,	"SQX_R_m/S");
		outTree->Branch("SQY_L_m",	&SQY_L_mult,	"SQY_L_m/S");
		outTree->Branch("SQY_R_m",	&SQY_R_mult,	"SQY_R_m/S");
		outTree->Branch("CsI_L_m",	&CsI_L_mult,	"CsI_L_m/S");
		outTree->Branch("CsI_R_m",	&CsI_R_mult,	"CsI_R_m/S");

		outTree->Branch("tSQX_L",	out_tSQX_L,		"tSQX_L[32]/D");
		outTree->Branch("tSQX_R",	out_tSQX_R,		"tSQX_R[32]/D");
		outTree->Branch("tSQY_L",	out_tSQY_L,		"tSQY_L[16]/D");
		outTree->Branch("tSQY_R",	out_tSQY_R,		"tSQY_R[16]/D");
		outTree->Branch("tCsI_L",	out_tCsI_L,		"tCsI_L[16]/D");
		outTree->Branch("tCsI_R",	out_tCsI_R,		"tCsI_R[16]/D");

		outTree->Branch("SQY_L_e",	SQY_L_Edep,		"SQY_L_e[16]/D");
		outTree->Branch("SQY_R_e",	SQY_R_Edep,		"SQY_R_e[16]/D");
		outTree->Branch("SQX_L_e",	SQX_L_Edep,		"SQX_L_e[32]/D");
		outTree->Branch("SQX_R_e",	SQX_R_Edep,		"SQX_R_e[32]/D");
		outTree->Branch("CsI_L_e",	CsI_L_Edep,		"CsI_L_e[16]/D");
		outTree->Branch("CsI_R_e",	CsI_R_Edep,		"CsI_R_e[16]/D");

		outTree->Branch("SQX_L_s",	SQX_L_strip,	"SQX_L_s[32]/S");
		outTree->Branch("SQX_R_s",	SQX_R_strip,	"SQX_R_s[32]/S");
		outTree->Branch("SQY_L_s",	SQY_L_strip,	"SQY_L_s[16]/S");
		outTree->Branch("SQY_R_s",	SQY_R_strip,	"SQY_R_s[16]/S");
		outTree->Branch("CsI_L_s",	CsI_L_strip,	"CsI_L_s[16]/S");
		outTree->Branch("CsI_R_s",	CsI_R_strip,	"CsI_R_s[16]/S");
	}
	


	outTree->Branch("SQX_L_sNo",	&SQX_L_stripNo,	"SQX_L_sNo/F");
	outTree->Branch("SQX_R_sNo",	&SQX_R_stripNo,	"SQX_R_sNo/F");
	outTree->Branch("SQY_L_sNo",	&SQY_L_stripNo,	"SQY_L_sNo/F");
	outTree->Branch("SQY_R_sNo",	&SQY_R_stripNo,	"SQY_R_sNo/F");

	//LEFT
	outTree->Branch("sqlde",		&sqlde,		"sqlde/D");
	outTree->Branch("sqletot",		&sqletot,	"sqletot/D");

	if (longV)
	{
		outTree->Branch("sqlphi",		&sqlphi,	"sqlphi/D");
		outTree->Branch("sqltheta",		&sqltheta,	"sqltheta/D");

		outTree->Branch("sqlang",		&sqlang,	"sqlang/D");
		outTree->Branch("sqlangCM",		&sqlangCM,	"sqlangCM/D");
		outTree->Branch("sqlang1",		&sqlang1,	"sqlang1/D");
		outTree->Branch("sqlang2",		&sqlang2,	"sqlang2/D");
		outTree->Branch("sqlang3",		&sqlang3,	"sqlang3/D");
		outTree->Branch("sqlxtime",		&sqlxtime,	"sqlxtime/D");
		outTree->Branch("sqlytime",		&sqlytime,	"sqlytime/D");
		outTree->Branch("fsqltheta_1H",	&fsqltheta_1H,"fsqltheta_1H/D");
		outTree->Branch("fsqltheta_2H",	&fsqltheta_2H,"fsqltheta_2H/D");
		outTree->Branch("kinsqle_1H",	&kinsqle_1H,"kinsqle_1H/D");
		outTree->Branch("kinsqle_2H",	&kinsqle_2H,"kinsqle_2H/D");
	}

	outTree->Branch("sqrde",		&sqrde,		"sqrde/D");
	outTree->Branch("sqretot",		&sqretot,	"sqretot/D");

	if (longV)
	{
		outTree->Branch("sqrtheta",		&sqrtheta,	"sqrtheta/D");
		outTree->Branch("sqrphi",		&sqrphi,	"sqrphi/D");
		outTree->Branch("sqrang",		&sqrang,	"sqrang/D");
		outTree->Branch("sqrang1",		&sqrang1,	"sqrang1/D");
		outTree->Branch("sqrang2",		&sqrang2,	"sqrang2/D");
		outTree->Branch("sqrang3",		&sqrang3,	"sqrang3/D");
		outTree->Branch("sqrxtime",		&sqrxtime,	"sqrxtime/D");
		outTree->Branch("sqrytime",		&sqrytime,	"sqrytime/D");
		outTree->Branch("fsqrtheta1",	&fsqrtheta1,	"fsqrtheta1/D");
		outTree->Branch("fsqrtheta2",	&fsqrtheta2,	"fsqrtheta2/D");

		outTree->Branch("s_csir",		&memS_CsI_R,	"s_csir/S");
		outTree->Branch("s_csil",		&memS_CsI_L,	"s_csil/S");

		outTree->Branch("X2H",	&X2H,	"X2H/F");
		outTree->Branch("Y2H",	&Y2H,	"Y2H/F");
		outTree->Branch("Z2H",	&Z2H,	"Z2H/F");

		outTree->Branch("X6He",	&X6He,	"X6He/F");
		outTree->Branch("Y6He",	&Y6He,	"Y6He/F");
		outTree->Branch("Z6He",	&Z6He,	"Z6He/F");

		outTree->Branch("evX",	&evX,	"evX/F");
		outTree->Branch("evY",	&evY,	"evY/F");
		outTree->Branch("evZ",	&evZ,	"evZ/F");
	}


	outTree->Branch("MWPC_1_X",	&MWPC_1_X,	"MWPC_1_X/F");
	outTree->Branch("MWPC_1_Y",	&MWPC_1_Y,	"MWPC_1_Y/F");
	outTree->Branch("MWPC_1_Z",	&MWPC_1_Z,	"MWPC_1_Z/F");

	outTree->Branch("MWPC_2_X",	&MWPC_2_X,	"MWPC_2_X/F");
	outTree->Branch("MWPC_2_Y",	&MWPC_2_Y,	"MWPC_2_Y/F");
	outTree->Branch("MWPC_2_Z",	&MWPC_2_Z,	"MWPC_2_Z/F");

	if (longV)
	{
		outTree->Branch("f6X",	&f6X,	"f6X/F");
		outTree->Branch("f6Y",	&f6Y,	"f6Y/F");
		outTree->Branch("f6Z",	&f6Z,	"f6Z/F");

		outTree->Branch("f5X",	&f5X,	"f5X/F");
		outTree->Branch("f5Y",	&f5Y,	"f5Y/F");
		outTree->Branch("f5Z",	&f5Z,	"f5Z/F");

		outTree->Branch("tF3",	out_tF3,	"tF3[4]/D");
		outTree->Branch("F3",	out_F3,		"F3[4]/D");
		outTree->Branch("tF5",	out_tF5,	"tF5[4]/D");
		outTree->Branch("F5",	out_F5,		"F5[4]/D");
		outTree->Branch("tF6",	out_tF6,	"tF6[4]/D");
		outTree->Branch("F6",	out_F6,		"F6[4]/D");


		outTree->Branch("tof",	&out_tof,	"tof/D");
		outTree->Branch("aF5",	&out_aF5,	"aF5/D");
		outTree->Branch("az",	&out_AZ,	"az/D");


		outTree->Branch("ion",	&Ion_ID,	"ion/S");
		outTree->Branch("run",	&Run_ID,	"run/S");

		outTree->Branch("x1",	out_x1,		"x1[32]/s");
		outTree->Branch("x2",	out_x2,		"x2[32]/s");
		outTree->Branch("y1",	out_y1,		"y1[32]/s");
		outTree->Branch("y2",	out_y2,		"y2[32]/s");

		outTree->Branch("nx1",	&out_nx1,	"nx1/s");
		outTree->Branch("nx2",	&out_nx2,	"nx2/s");
		outTree->Branch("ny1",	&out_ny1,	"ny1/s");
		outTree->Branch("ny2",	&out_ny2,	"ny2/s");

		outTree->Branch("tMWPC",	out_tMWPC,	"tMWPC[4]/F");	
		outTree->Branch("trigger",	&out_trig,	"trigger/S");
		outTree->Branch("mml1",	&mml1,	"mml1/D");
		outTree->Branch("mml2",	&mml2,	"mml2/D");

		outTree->Branch("c_SQX_L",	c_SQX_L,	"c_SQX_L[32]/D");
		outTree->Branch("c_SQX_R",	c_SQX_R,	"c_SQX_R[32]/D");
		outTree->Branch("c_SQY_L",	c_SQY_L,	"c_SQY_L[16]/D");
		outTree->Branch("c_SQY_R",	c_SQY_R,	"c_SQY_R[16]/D");
		outTree->Branch("c_CsI_L",	c_CsI_L,	"c_CsI_L[16]/D");
		outTree->Branch("c_CsI_R",	c_CsI_R,	"c_CsI_R[16]/D");

		outTree->Branch("r_SQX_L",	raw_SQX_L,	"r_SQX_L[32]/D");
		outTree->Branch("r_SQX_R",	raw_SQX_R,	"r_SQX_R[32]/D");
		outTree->Branch("r_SQY_L",	raw_SQY_L,	"r_SQY_L[16]/D");
		outTree->Branch("r_SQY_R",	raw_SQY_R,	"r_SQY_R[16]/D");
		outTree->Branch("r_CsI_L",	raw_CsI_L,	"r_CsI_L[16]/D");
		outTree->Branch("r_CsI_R",	raw_CsI_R,	"r_CsI_R[16]/D");
	}

	outTree->Branch("he4",	&he4,	"he4/O");
	outTree->Branch("he6",	&he6,	"he6/O");

	outTree->Branch("kineProB",	&kineProB,	"kineProB/O");
	outTree->Branch("kineDeuB",	&kineDeuB,	"kineDeuB/O");

	outTree->Branch("geo",	&Geo_ID,	"geo/S");
	outTree->Branch("deu",	&angAng2H,	"deu/O");
	outTree->Branch("pro",	&angAng1H,	"pro/O");
	outTree->Branch("dEAngle",	&dEAngle,	"dEAngle/O");

	if (longV)
	{
		outTree->Bronch("LV_6He.",	"TLorentzVector",	&LV_6He);
		outTree->Bronch("LV_2H.",	"TLorentzVector",	&LV_2H);
	}

	outTree->Branch("kinE",	&kinE,	"kinE/D");
	outTree->Bronch("LV_beam.",	"TLorentzVector",	&LV_beam);

	return true;
}

Float_t eAngle::actual_work(Double_t inputPars[4], Double_t qualityControl[4], Int_t geoNo)
{
	Long64_t nEntries = inTree->GetEntries();
	double inPar[6];
	std::fill(inPar,inPar+6,0.0);
	
	inPar[0] = 0.0;//4.22389e+00;
	inPar[1] = 0.0;//9.33719e+00;
	inPar[2] = 0.0;//9.23913e+00;
	inPar[3] = 0.0;//-2.60958e+00;
	inPar[4] = 0.0;//-1.41267e+00;
	inPar[5] = 0.0;//-4.84871e+00;

	SQL_dist = 170.0 + cs::leftDetDist;
	SQR_dist = 250.0 + cs::rightDetDist;

	switch (geoNo)
	{
	case 0:
	{
		printf("Wrong geometry\n");
		SQL_ang = (0.0) * TMath::DegToRad();
		SQR_ang = (0.0) * TMath::DegToRad();
		tarPos = 0.0;
		break;
	}

	case 1:
		{
			tarPos = cs::tarPos;
			tarThickness = 80.0 + cs::tarThicknessShift;
			tarAngle = 45.0 * TMath::DegToRad();
			SQL_ang = (65.0 + cs::leftAngShift1) * TMath::DegToRad();
			SQR_ang = (15.0 + cs::rightAngShift1) * TMath::DegToRad();
			break;
		}

	case 2:
		{
			tarPos = cs::tarPos;
			tarThickness = 160 + 2*cs::tarThicknessShift;
			tarAngle = 6.0 * TMath::DegToRad();
			SQL_ang = (50.0 + cs::leftAngShift2) * TMath::DegToRad();
			SQR_ang = (15.0 + cs::rightAngShift2) * TMath::DegToRad();
			break;
		}

	case 3:
		{
			tarPos = cs::tarPos;
			tarThickness = 160 + 2*cs::tarThicknessShift;
			tarAngle = 0.0 * TMath::DegToRad();
			SQL_ang = (35.0 + cs::leftAngShift3) * TMath::DegToRad();
			SQR_ang = (15.0 + cs::rightAngShift3) * TMath::DegToRad();
			break;
		}

	}

	for (Long64_t entry=0; entry<nEntries; entry++)
	{

		inTree->GetEntry(entry);
		//NULLing everything
		memE_CsI_R=0.0; memE_CsI_L=0.0; memS_CsI_R=20; memS_CsI_L=20;
		kine_2H=0.0, kine_1H=0.0;
		Ion_ID = 0, Geo_ID = geoNo, kinE = 0.0;
		evX=0.0;evY=0.0;evZ=0.0; Run_ID = run;

		he4=false, he6=false, angAng1H=false, angAng2H=false, dEAngle=false;
		kineProB = false;
		kineDeuB = false;

		X6He=0.0; Y6He=0.0; Z6He=0.0; X2H=0.0; Y2H=0.0; Z2H=0.0;
		sqrphi=0.0; sqrtheta=0.0; sqrang=0.0; sqrde=0.0; sqretot=0.0; sqlangCM=0.0;
		sqlphi=0.0; sqltheta=0.0; sqlang=0.0; sqlde=0.0; sqletot=0.0;

		SQX_L_mult=0; SQY_L_mult=0; CsI_L_mult=0;
		SQX_R_mult=0; SQY_R_mult=0; CsI_R_mult=0;

		out_aF5 = in_aF5;
		out_nx1 = in_nx1;
		out_nx2 = in_nx2;
		out_ny1 = in_ny1;
		out_ny2 = in_ny2;
		out_trig = in_trig;
		out_tof = in_tof;
		out_AZ = in_AZ;

		//4 channels loop
		for (int iii=0; iii<4; iii++)
		{
			out_tF3[iii]=in_tF3[iii];
			out_F3[iii]=in_F3[iii];
			out_tF5[iii]=in_tF5[iii];
			out_F5[iii]=in_F5[iii];
			out_tF6[iii]=in_tF6[iii];
			out_F6[iii]=in_F6[iii];
			out_tMWPC[iii] = in_tMWPC[iii];
		}
	
		//16 channels loop
		for (int iii=0; iii<16; iii++)
		{
			if (SQY_L[iii]>1.0)
			{
				SQY_L_mult++;
				SQY_L_Edep[SQY_L_mult]=SQY_L[iii];
				SQY_L_strip[SQY_L_mult]=iii;
			}
	
			if (SQY_R[iii]>5.0)
			{
				SQY_R_mult++;
				SQY_R_Edep[SQY_R_mult]=SQY_R[iii];
				SQY_R_strip[SQY_R_mult]=iii;
			}

			if (in_tCsI_R[iii]>cs::tc_CsI_R)
			{
				if (memE_CsI_R<CsI_R[iii])
				{
					memE_CsI_R = CsI_R[iii];
					memS_CsI_R = iii;
				}

				CsI_R_mult++;
				CsI_R_Edep[CsI_R_mult]=CsI_R[iii];
				CsI_R_strip[CsI_R_mult]=iii;
			}

			if (in_tCsI_L[iii]>cs::tc_CsI_L)
			{
				if (memE_CsI_L<CsI_L[iii])
				{
					memE_CsI_L = CsI_L[iii];
					memS_CsI_L = iii;
				}

				CsI_L_mult++;
				CsI_L_Edep[CsI_L_mult]=CsI_L[iii];
				CsI_L_strip[CsI_L_mult]=iii;
			}

			c_SQY_L[iii]=SQY_L[iii];
			c_SQY_R[iii]=SQY_R[iii];
			c_CsI_L[iii]=CsI_L[iii];
			c_CsI_R[iii]=CsI_R[iii];

			raw_SQY_L[iii]=r_SQY_L[iii];
			raw_SQY_R[iii]=r_SQY_R[iii];
			raw_CsI_L[iii]=r_CsI_L[iii];
			raw_CsI_R[iii]=r_CsI_R[iii];

			out_tCsI_L[iii]=in_tCsI_L[iii];
			out_tCsI_R[iii]=in_tCsI_R[iii];
			out_tSQY_L[iii]=in_tSQY_L[iii];
			out_tSQY_R[iii]=in_tSQY_R[iii];
		}

		//32 channels loop
		for (int iii=0; iii<32; iii++)
		{
			out_x1[iii]=in_x1[iii];
			out_x2[iii]=in_x2[iii];
			out_y1[iii]=in_y1[iii];
			out_y2[iii]=in_y2[iii];

			if (SQX_L[iii]>1.0)
			{
				SQX_L_mult++;
				SQX_L_Edep[SQX_L_mult]=SQX_L[iii];
				SQX_L_strip[SQX_L_mult]=iii;
			}

			if (SQX_R[iii]>5.0)
			{
				SQX_R_mult++;
				SQX_R_Edep[SQX_R_mult]=SQX_R[iii];
				SQX_R_strip[SQX_R_mult]=iii;
			}

			out_tSQX_L[iii]=in_tSQX_L[iii];
			out_tSQX_R[iii]=in_tSQX_R[iii];
			raw_SQX_L[iii]=r_SQX_L[iii];
			raw_SQX_R[iii]=r_SQX_R[iii];
			c_SQX_L[iii]=SQX_L[iii];
			c_SQX_R[iii]=SQX_R[iii];
		}

		//Get position of the center of the hit in MWPC in milimeters
		maynard->Get_MWPC_pos(in_nx1, in_x1, &MWPC_1_X, cs::MWPC_1_X_id);
		maynard->Get_MWPC_pos(in_ny1, in_y1, &MWPC_1_Y, cs::MWPC_1_Y_id);
		maynard->Get_MWPC_pos(in_nx2, in_x2, &MWPC_2_X, cs::MWPC_2_X_id);
		maynard->Get_MWPC_pos(in_ny2, in_y2, &MWPC_2_Y, cs::MWPC_2_Y_id);

		//smear the cluster to the full width of the strip (1.25 mm)
		Double_t fMWPC_1_X, fMWPC_1_Y, fMWPC_1_Z = -816.0, fMWPC_2_X, fMWPC_2_Y, fMWPC_2_Z = -270.0;
		MWPC_1_X += rnd->Uniform(0.0,1.25)-0.625;
		MWPC_1_Y += rnd->Uniform(0.0,1.25)-0.625;
		MWPC_2_X += rnd->Uniform(0.0,1.25)-0.625;
		MWPC_2_Y += rnd->Uniform(0.0,1.25)-0.625;
		MWPC_1_Z = -816.0;
		MWPC_2_Z = -270.0;

		//add displacement from the minimization
		fMWPC_1_X = MWPC_1_X + cs::MWPC1_X_displacement;
		fMWPC_1_Y = MWPC_1_Y + cs::MWPC1_Y_displacement;		
		fMWPC_2_X = MWPC_2_X + cs::MWPC2_X_displacement;
		fMWPC_2_Y = MWPC_2_Y + cs::MWPC2_Y_displacement;
		
		//get vector components
		dX=fMWPC_2_X-fMWPC_1_X;
		dY=fMWPC_2_Y-fMWPC_1_Y;
		dZ=fMWPC_2_Z-fMWPC_1_Z;

		//get position of the hit
		vBeam->SetXYZ(dX,dY,dZ);
		TVector3 tarPoint(0.0, 0.0, tarPos);
		TVector3 beamPoint(fMWPC_2_X, fMWPC_2_Y, fMWPC_2_Z);
		TVector3 tarPerpendicular(sin(tarAngle), 0.0, cos(tarAngle));
		double dCoeff = ((tarPoint-beamPoint).Dot(tarPerpendicular))/(vBeam->Dot(tarPerpendicular));
		
		evX = fMWPC_2_X + dX * dCoeff;
		evY = fMWPC_2_Y + dY * dCoeff;
		evZ = fMWPC_2_Z + dZ * dCoeff;

		//get kinetic energy of the beam
		if ((in_aF5>600 && in_aF5<1200 && in_tof>160 && in_tof<182) || (in_aF5==0 && in_tof>160 && in_tof<182))
		{
			kinE = maynard->getT(in_tof,cs::mass6He);
			kinE = he6_Si->GetE(kinE, ToFMWPC);
			Ion_ID = 62;
		}

		//create Lorentz Vector of the beam
		ene_beam = cs::mass6He + kinE;
		mom_beam = sqrt(ene_beam*ene_beam - cs::mass6He*cs::mass6He);

		vBeam->SetMag(mom_beam);
		LV_beam->SetVectM(*vBeam, cs::mass6He);

		//create kinematics for p,p and p,t reactions
		TLorentzVector lv6He_EL(*LV_beam);
		TLorentzVector lv1H_EL(0,0,0,cs::mass1H);
		TLorentzVector lvCM_EL = lv6He_EL+lv1H_EL;
		TVector3 boostVect_EL = lvCM_EL.BoostVector();

		lv6He_EL.Boost(-boostVect_EL);
		lv1H_EL.Boost(-boostVect_EL);

		double theta_CM = rnd->Uniform(0.0,TMath::Pi());

		Double_t eneCM	= lv1H_EL.E() + lv6He_EL.E() + 7.51;
		Double_t ene3H	= (1/(2*eneCM))*(eneCM*eneCM-cs::mass4He*cs::mass4He+cs::mass3H*cs::mass3H);
		Double_t ene4He	= (1/(2*eneCM))*(eneCM*eneCM+cs::mass4He*cs::mass4He-cs::mass3H*cs::mass3H);
		Double_t mom3H	= sqrt(ene3H*ene3H-cs::mass3H*cs::mass3H);
		Double_t mom4He = sqrt(ene4He*ene4He-cs::mass4He*cs::mass4He);

		TLorentzVector lv3H(0.0,0.0,mom3H, cs::mass3H);
		TLorentzVector lv4He(0.0,0.0,mom4He, cs::mass4He);
		lv3H.SetTheta(TMath::Pi()-theta_CM);
		lv4He.SetTheta(theta_CM);

		lv3H.Boost(boostVect_EL);
		lv4He.Boost(boostVect_EL);
		sqlang3 = 180.0 * (LV_beam->Angle(lv3H.Vect()))/double(TMath::Pi());
		sqrang3 = 180.0 * (LV_beam->Angle(lv4He.Vect()))/double(TMath::Pi());


		lv6He_EL = *LV_beam;
		TLorentzVector lv2H_EL(0.0,0.0,0.0,cs::mass2H);
		lvCM_EL = lv6He_EL+lv2H_EL;
		Double_t gammaSquare2H = lvCM_EL.Gamma()*lvCM_EL.Gamma();
		Double_t m_ThetaCM2H = lvCM_EL.Theta();
		boostVect_EL = lvCM_EL.BoostVector();

		lv6He_EL.Boost(-boostVect_EL);
		lv2H_EL.Boost(-boostVect_EL);

		lv6He_EL.SetTheta(TMath::Pi()-theta_CM);
		lv2H_EL.SetTheta(theta_CM);

		lv6He_EL.Boost(boostVect_EL);
		lv2H_EL.Boost(boostVect_EL);
		sqlang2 = 180.0 * (LV_beam->Angle(lv2H_EL.Vect()))/double(TMath::Pi());
		sqrang2 = 180.0 * (LV_beam->Angle(lv6He_EL.Vect()))/double(TMath::Pi());

		TVector3 vectH;

		//SQL energy - deuterium
		if (SQX_L_mult == 1 && SQY_L_mult == 1 && kinE>0)
		{
			//smear strip
			SQX_L_stripNo = SQX_L_strip[1]+rnd->Uniform(0.0,1.0)-0.5;
			SQY_L_stripNo = SQY_L_strip[1]+rnd->Uniform(0.0,1.0)-0.5;

			// coordinates of hit in LAB system
			X2H_lab = SQL_dist*sin(SQL_ang) + (cs::SQLstartX + cs::leftDetShift) * cos(SQL_ang);
			Y2H_lab = cs::SQLstartY + cs::widthStripY;
			Z2H_lab = SQL_dist*cos(SQL_ang) - (cs::SQLstartX + cs::leftDetShift) * sin(SQL_ang);

			X2H_det	=	-cs::widthStripX	*	(SQX_L_strip[1]+rnd->Uniform(0.0,1.0)-0.5) * cos(SQL_ang);
			Y2H_det	=	cs::widthStripY	* 	(SQY_L_strip[1]+rnd->Uniform(0.0,1.0)-0.5);
			Z2H_det	=	cs::widthStripX	*	(SQX_L_strip[1]+rnd->Uniform(0.0,1.0)-0.5) * sin(SQL_ang);

			X2H = X2H_lab + X2H_det;
			Y2H = Y2H_lab + Y2H_det;
			Z2H = Z2H_lab + Z2H_det;

			vectH.SetXYZ(X2H-evX, Y2H-evY, Z2H-evZ);
			sqlphi=vectH.Phi()*TMath::RadToDeg();
			sqltheta=vectH.Theta()*TMath::RadToDeg();
			sqlang = vectH.Angle(*vBeam)*TMath::RadToDeg();
			Double_t tanSquare = tan(vectH.Angle(*vBeam)) * tan(vectH.Angle(*vBeam));
			Double_t cosLeftAng = (1.0 - gammaSquare2H*tanSquare)/(1 + gammaSquare2H*tanSquare);
			sqlangCM = (TMath::Pi() - (acos(cosLeftAng)-m_ThetaCM2H))*TMath::RadToDeg();
		}

		//SQR energy - helium
		if(SQX_R_mult == 1 && SQY_R_mult == 1 && kinE>0.0)
		{
			SQX_R_stripNo = SQX_R_strip[1]+rnd->Uniform(0.0,1.0)-0.5;
			SQY_R_stripNo = SQY_R_strip[1]+rnd->Uniform(0.0,1.0)-0.5;
			sqrde=SQY_R_Edep[1];
			sqretot=memE_CsI_R;
			s_csir = memS_CsI_R;

			// coordinates of hit in LAB system
			X6He_lab = SQR_dist*(sin(-SQR_ang)) - (cs::SQRstartX + cs::rightDetShift) * cos(SQR_ang);
			Y6He_lab = cs::SQRstartY;
			Z6He_lab = SQR_dist*cos(SQR_ang) - (cs::SQRstartX + cs::rightDetShift) * sin(SQR_ang);		

			X6He_det = cs::widthStripX * SQX_R_stripNo * cos(SQR_ang);
			Y6He_det = cs::widthStripY * SQY_R_stripNo;
			Z6He_det = cs::widthStripX * SQX_R_stripNo * sin(SQR_ang);

			X6He = X6He_lab + X6He_det;
			Y6He = Y6He_lab + Y6He_det;
			Z6He = Z6He_lab + Z6He_det;

			//setting deuterium vector
			TVector3 vect6He(X6He-evX, Y6He-evY, Z6He-evZ);
			ene_6He = sqrde + sqretot + cs::mass6He;
			mom_6He = sqrt(ene_6He*ene_6He-cs::mass6He*cs::mass6He);
			vect6He.SetMag(mom_6He);

			sqrtheta=vect6He.Theta()*TMath::RadToDeg();
			sqrphi=vect6He.Phi()*TMath::RadToDeg();
			sqrang = vect6He.Angle(*vBeam)*TMath::RadToDeg();

		}

		if (gcut_dehe4->IsInside(sqretot,sqrde)) he4=true;
		if (gcut_dehe6->IsInside(sqretot,sqrde)) he6=true;

		if ((he4 || he6) && SQX_L_Edep[1]>0 && kinE>0)
		{
			if (gcut_AngAng1H_w6He->IsInside(sqrang, sqlang))
			{
				angAng1H=true;
			}

			else if (gcut_AngAng2H_w6He->IsInside(sqrang, sqlang))
			{
				angAng2H=true;
			}
			
		}
				
		if (sqlang>0 && angAng1H)
		{
			sqlde = SQX_L_Edep[1];
			//sqlde	=	h1_Si->GetE(SQX_L_Edep[1], -3.5);
			//sqlde	=	h1_CD2->GetE(sqlde, -tarThickness/2);
			sqletot = memE_CsI_L;
			s_csil = memS_CsI_L;
			ene_1H = sqlde + cs::mass1H;
			mom_1H = sqrt(ene_1H*ene_1H-cs::mass1H*cs::mass1H);

			vectH.SetMag(mom_1H);
			LV_1H->SetVectM(vectH, cs::mass1H);
			*LV_6He = LV_tar_1H + *LV_beam - *LV_1H;
			mml1 = LV_6He->M() - cs::mass6He;
			kinsqle_1H = maynard->kin_GetERecoil(vectH.Angle(*vBeam), kinE, cs::mass1H/cs::mass6He);
			fsqltheta_1H = 180.0*atan(sin(2*vBeam->Angle(vectH))/(-cos(2*vBeam->Angle(vectH))+cs::mass6He/cs::mass1H))/TMath::Pi();
			fsqrtheta1 = maynard->kin_GetAngProjectile(kinsqle_1H, kinE, cs::mass1H/cs::mass6He);
			
			if (dEAnglePro->IsInside(sqlang, sqlde) && geoNo==1)
			{
				dEAngle = true;
			}
		}

		else if (sqlang>0 && angAng2H)
		{
			sqlde = SQX_L_Edep[1];
			//sqlde	=	h2_Si->GetE(SQX_L_Edep[1], -3.5);
			//sqlde	=	h2_CD2->GetE(sqlde, -tarThickness/2);
			sqletot = memE_CsI_L;
			s_csil = memS_CsI_L;
			ene_2H = sqlde + cs::mass2H;
			mom_2H = sqrt(ene_2H*ene_2H-cs::mass2H*cs::mass2H);
			vectH.SetMag(mom_2H);
			LV_2H->SetVectM(vectH, cs::mass2H);
			*LV_6He = LV_tar_2H + *LV_beam - *LV_2H;
			mml2 = LV_6He->M() - cs::mass6He;
			kinsqle_2H = maynard->kin_GetERecoil(vectH.Angle(*vBeam), kinE, cs::mass2H/cs::mass6He);
			fsqltheta_2H = 180.0*atan(sin(2*vBeam->Angle(vectH))/(-cos(2*vBeam->Angle(vectH))+cs::mass6He/cs::mass2H))/TMath::Pi();
			fsqrtheta2 = maynard->kin_GetAngProjectile(kinsqle_2H, kinE, cs::mass2H/cs::mass6He);
			
			if (dEAngleDeu->IsInside(sqlang, sqlde) && geoNo==1)
			{
				dEAngle = true;
			}
		}
		sqldEE = sqlde + sqletot;
		//printf("%f\t%f\n", sqlang, sqlde+sqletot);
		if (proKine->IsInside(sqldEE, sqlang)) kineProB = true;
		if (deuKine->IsInside(sqldEE, sqlang)) kineDeuB = true;

		if (/*he6 && (proKine->IsInside(sqlang, sqlde+sqletot) || deuKine->IsInside(sqlang, sqlde+sqletot))*/1)
		{
			outTree->Fill();
		}
		

	}
	return 3.3;
}