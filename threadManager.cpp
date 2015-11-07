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

	initWorkerThreads();

	// while(!v_requestBuffer.empty()){
	// 	dequeueRequestBuffer(v_requestBuffer.back());
	// 	v_requestBuffer.pop_back();
	// }

	// requestThread1.join();
	// requestThread2.join();
	// requestThread3.join();

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

void ThreadManager::dequeueRequestBuffer()
{
	string strServerThreadRequest = m_controlChannel->send_request("newthread");
	cout << "Reply to request 'newthread' is " << strServerThreadRequest << "'" << endl;
	RequestChannel chan2(strServerThreadRequest, RequestChannel::CLIENT_SIDE);

	while(!v_requestBuffer->isDone()){
		RequestPackage newPackage = v_requestBuffer->V();

		string strReply = chan2.send_request("data " + newPackage.personRequested);
		newPackage.serverResponse = strReply;

		if (newPackage.personRequested == "Joe Smith"){
			v_responseBuffer1->P(newPackage);
		}
		else if (newPackage.personRequested == "Jane Smith"){
			v_responseBuffer2->P(newPackage);
		}
		else if (newPackage.personRequested == "John Doe"){
			v_responseBuffer3->P(newPackage);
		}
	}

	// string threadRequest = m_controlChannel->send_request("newthread");
	// cout << "Reply to request 'newthread' is " << threadRequest << "'" << endl;
	// RequestChannel chan2(threadRequest, RequestChannel::CLIENT_SIDE);
	//
	// string reply6 = chan2.send_request("data "+rqstPackg.personRequested);
	// cout << "Reply to request 'data " + rqstPackg.personRequested + "' is '" << reply6 << "'" << endl;
	//
	// string reply7 = chan2.send_request("quit");
	// cout << "Reply to request 'quit' is '" << reply7 << "'" << endl;

}

void ThreadManager::initRequestThreads(){

}

void ThreadManager::initWorkerThreads(){
	for (int i = 0; i < m_numberOfWorkers; ++i){
		v_workerThreads.push_back(std::thread(&ThreadManager::dequeueRequestBuffer, this));
	}
}

void ThreadManager::initStatisticsThreads(){

}


void ThreadManager::joinRequestThreads(){

}

void ThreadManager::joinWorkerThreads(){
	for (int i = 0; i < v_workerThreads.size(); ++i)
	{
		v_workerThreads[i].join();
	}
}

void ThreadManager::joinStatisticsThreads(){

}
