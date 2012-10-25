#pragma once
#include "Bpb.h"

class Fat16
{
public:
	Fat16(void);
	~Fat16(void);

	void load(BYTE* bufMbr);

	//�n���@��type
	int type;

	ULONG jmpBoot;
	CString OEMName;

	WORD bpb_BytesPerSec;
	BYTE bpb_SecPerClu;
	WORD bpb_RsvdSecCnt;

	//Fat�����ơA�q�`������A�ت��O���F�H���U�@
	BYTE bpb_NumFats;
	WORD bpb_RootEntCnt;

	//13H��14H �o�Ӧr�ժ���޿�ϰ��`�ơA���̦h�u���� 0FFFFH
	//�ҥH�Y�@�ӺϺЪ��޿�ϰϤj�� 0FFFFH ( �άO���ϺЮe�q�j�� 32MB�A32MB �O 10000H*512D �Ӫ� )
	//�h����쬰 0�A�޿�ϰ��`�ưO���b 20H �B�}�l�����r�աC
	//�]�� FAT32 ���Υ��w�j�� 32MB�A�G����줣�ϥΡA����J 0�C
	WORD bpb_TotSec16;

	//��} 15H �o�Ӧ줸�ժ�ܺϺк����C�����M FAT ���Ĥ@�Ӧ줸�դ@�ˡA��N�q�p�U��G
	//0F0H�G1.44MB �T�T�b�n��	0F8H�G�w��
	//0F9H�G1.2M 5.25 �T�n��	0FDH�G360K 5.25 �T�n��
	//0FEH�G160K 5.25 �T�n��	0FFH�G320K 5.25 �T�n��
	BYTE bpb_Media;

	//�C�� FAT �Ҧ��ϰϼơF���� FAT32 �Ө��A����줣�ΡA��W 0�A
	WORD bpb_FatSz16;

	//�N��C�@�ϭy���ϰϼ�
	WORD bpb_SecPerTrk;

	//�N����Y��
	WORD bpb_NumHeads;

	//�o�����r�� (�@4�Ӧ줸��) ������úϰϼơA�P���Ϊ���X�R���Ϊ������ΰϬ۹��m���ƭȤ@��
	ULONG bpb_HiddSec;

	//�o�����r��(�@4�Ӧ줸��) �N���޿�ϰ��`�ơC�p�G 16H��17H �欰 0�A�h���椣�i���s�C
	ULONG bpb_TotSec32;

	BYTE drvNum;
	BYTE reserved1;
	BYTE bootSig;
	CString volID;
	CString volLab;
	CString filSysType;


	ULONG getTotalSec();
	ULONG getFatSize();
	ULONG getDataSec();
	ULONG getRootDirSectors();
	ULONG getTotalCluster();

	void toAry(BYTE* array16);

	void loadNewFat16(ULONG totalSec, int sizeMB, int hiddenSec);
	int getSecPerFat(int TS);
	//int getDskSize(int sizeMb);
	
	void getHS( int& h, int& s, int sizeMB);

private:
	int _getSecPerCluster(int sizeMB);
	
};
