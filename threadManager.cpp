#include "threadManager.h"

ThreadManager::ThreadManager(int RequestsPerPerson, int SizeOfBuffer, int NumberOfWorkers)
{

	m_requestsPerPerson = RequestsPerPerson;
	m_sizeOfBuffer = SizeOfBuffer;
	m_numberOfWorkers = NumberOfWorkers;
	printf("This application supports:\n\tRequests Per Person: %d\n\tBuffer Size: %d\n\tTotal Worker Threads: %d\n",m_requestsPerPerson, m_sizeOfBuffer, m_numberOfWorkers);

	printf("Establishing control channel... ");
	m_controlChannel = new RequestChannel("control", RequestChannel::CLIENT_SIDE);
    printf("done.\n");

	v_requestBuffer = new Semaphore(m_sizeOfBuffer);
	v_responseBuffer1 = new Semaphore(m_sizeOfBuffer);
	v_responseBuffer2 = new Semaphore(m_sizeOfBuffer);
	v_responseBuffer3 = new Semaphore(m_sizeOfBuffer);
}

ThreadManager::~ThreadManager()
{
	string reply = m_controlChannel->send_request("quit");
	cout << "Reply to request 'quit' is '" << reply << "'" << endl;

	delete v_requestBuffer;
	delete v_responseBuffer1;
	delete v_responseBuffer2;
	delete v_responseBuffer3;
}

void ThreadManager::StartClient()
{
	printf("Started Client\n");
	initRequestThreads();

	// while(!v_requestBuffer.empty()){
	// 	dequeueRequestBuffer(v_requestBuffer.back());
	// 	v_requestBuffer.pop_back();
	// }


}

void ThreadManager::enqueueRequestBuffer(string personRequested)
{
	for(int requestNum = 1; requestNum < m_requestsPerPerson + 1; requestNum++)
	{
		RequestPackage rqstPckg;
		rqstPckg.personRequested = personRequested;
		rqstPckg.requestNumber = requestNum;
		rqstPckg.requestEnqued = clock();

		v_requestBuffer->P(rqstPckg);
		v_requestBuffer->V();
	}
}

void ThreadManager::dequeueRequestBuffer(RequestPackage rqstPackg)
{
	string threadRequest = m_controlChannel->send_request("newthread");
	cout << "Reply to request 'newthread' is " << threadRequest << "'" << endl;
	RequestChannel chan2(threadRequest, RequestChannel::CLIENT_SIDE);

	string reply6 = chan2.send_request("data "+rqstPackg.personRequested);
	cout << "Reply to request 'data " + rqstPackg.personRequested + "' is '" << reply6 << "'" << endl;

	string reply7 = chan2.send_request("quit");
	cout << "Reply to request 'quit' is '" << reply7 << "'" << endl;

}

void ThreadManager::initRequestThreads(){
	// enqueueRequestBuffer("Dimas Gonzales");
	std::thread requestThread1 (&ThreadManager::enqueueRequestBuffer, this, "Dimas Gonzales");
	std::thread requestThread2 (&ThreadManager::enqueueRequestBuffer,this, "Alejandro Suazo");
	std::thread requestThread3 (&ThreadManager::enqueueRequestBuffer,this, "John Jacob");
	requestThread1.join();
	requestThread2.join();
	requestThread3.join();


}



void ThreadManager::initWorkerThreads(){

}

void ThreadManager::initStatisticsThreads(){

}


void ThreadManager::joinRequestThreads(){

}

void ThreadManager::joinWorkerThreads(){

}

void ThreadManager::joinStatisticsThreads(){

}
