#pragma once
#include <atomic>
#include <thread>
#include <array>
#include <tuple>


struct rbStruct {
	int len;
	const char* ptr;
};
//UWAGA: Wszystkie urz�dznia typu Slave powinny by� zainicjowane przed zapisywaniem do bufora

class ringBuffer
{
private:
	std::atomic<char*> mp_master;				//Wska�nik dla w�tku zapisuj�cego (Master)
	std::array<std::atomic<char*>, 4> mp_slave;	//Wska�nik dla w�tku odczytuj�cego (Slave), jeden slave na jeden w�tek
	std::atomic<unsigned long long> m_byteCounter;
	std::atomic<int> m_fullBufferCount;
	std::atomic<int> m_minFreeMem;
	int m_slaveCount;
	const char* mp_memory;						//Wska�nik na cz�� pami�ci
	const int m_bufferSize;						//Rozmiar bufora
	const char* mp_endBuffer;					//Wska�nik ko�ca bufora


	int calcDistanceFromPtrToMaster(char* ptr);
	int calcDistanceFromMasterToPtr(char* ptr); //Oblicz odleg�o�� do wska�nika master lub ko�ca pami�ci.
	int calcRealFreeMemory();

	ringBuffer();
public:
	ringBuffer(const char* p_memory, int bufferSize, int slaveCount) :
		mp_master((char*)p_memory),
		m_byteCounter(0),
		m_fullBufferCount(0),
		m_minFreeMem(bufferSize),
		mp_memory(p_memory),
		m_slaveCount(slaveCount),
		m_bufferSize(bufferSize)
	{
		mp_endBuffer = mp_memory + bufferSize - 1;

		for (auto& p : mp_slave) {
			p.store((char*)nullptr);
		}
	};

	bool areSlavesInitialized();
	//Zwraca maksymaln� d�ugo�� do zapisu oraz wska�nik rozpoczynaj�cy
	rbStruct getMasterMemory();
	//Aktualizuje wska�nik o dan� ilo�� element�w
	void updateMaster(int len);

	//Inicjalizuje bufor dla Slave
	void initSlave(int slaveId);
	//Zwraca maksymaln� d�ugo�� do odczytu oraz wska�nik rozpoczynaj�cy
	rbStruct getSlaveMemory(int slaveId);
	//Aktualizuje wska�nik o dan� ilo�� element�w
	void updateSlave(int slaveId, int len);

	unsigned long long getByteCounter();
	int getFullBufferCount();
	int getBufferSize();
	int getMinFreeMem();
};
