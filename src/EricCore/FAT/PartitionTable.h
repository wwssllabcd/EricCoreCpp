#pragma once

class PartitionTable
{
public:
	PartitionTable(void);
	~PartitionTable(void);

	//啟動狀態
	bool isActive;
	BYTE headStart;
	WORD cylinderStart;
	BYTE sectorStart;
	
	//可得知該partition是哪種型態的分割，fat12,fat16 or fat32
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
