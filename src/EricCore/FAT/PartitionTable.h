#pragma once

class PartitionTable
{
public:
	PartitionTable(void);
	~PartitionTable(void);

	//�Ұʪ��A
	bool isActive;
	BYTE headStart;
	WORD cylinderStart;
	BYTE sectorStart;
	
	//�i�o����partition�O���ث��A�����ΡAfat12,fat16 or fat32
	BYTE partitionType;//04H

	BYTE headEnd;//05H
	BYTE sectorEnd;//06H 	
	WORD cylinderEnd;

	ULONG relativeSector;
	ULONG totalSec;

	void load(const BYTE* partitionTable);
	void toArray(BYTE* array);

	void loadNewPartition(int c, int h, int s);

private:
	//int _getSecPerTrack(int sizeMB);
	

};
