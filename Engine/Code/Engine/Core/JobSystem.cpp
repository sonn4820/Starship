#include "Engine/Core/JobSystem.hpp"

JobWorker::JobWorker(int id, JobSystem* system)
{
	m_id = id;
	m_system = system;
	m_thread = new std::thread(&JobWorker::ThreadMain, this);
}

JobWorker::~JobWorker()
{
	m_thread->join();
}

void JobWorker::ThreadMain()
{
	while (!m_system->m_isShuttingDown)
	{
		Job* jobToExecute = m_system->ClaimJob(this);
		if (jobToExecute)
		{
			jobToExecute->Execute();
			m_system->CompleteJob(jobToExecute);
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::microseconds(1));
		}
	}
}

JobSystem::JobSystem(JobSystemConfig config)
	:m_config(config)
{

}

void JobSystem::Startup()
{
	if (m_config.m_numWorkers < 0)
	{
		m_config.m_numWorkers = std::thread::hardware_concurrency() - 1;
	}
	CreateWorkers(m_config.m_numWorkers);
}

void JobSystem::BeginFrame()
{

}

void JobSystem::EndFrame()
{

}

void JobSystem::Shutdown()
{
	m_isShuttingDown = true;
	DestroyWorkers();
}

void JobSystem::CreateWorkers(int num)
{
	for (int i = 0; i < num; i++)
	{
		JobWorker* newWorker = new JobWorker(i, this);
		m_workers.push_back(newWorker);
	}
}

void JobSystem::DestroyWorkers()
{
	for (size_t i = 0; i < m_workers.size(); i++)
	{
		delete m_workers[i];
		m_workers[i] = nullptr;
	}

	m_workers.clear();
}

void JobSystem::QueueJob(Job* jobToQueue)
{
	m_queuedJobsMutex.lock();
	jobToQueue->m_state = JobState::QUEUED;
	m_queuedJobs.push_back(jobToQueue);
	m_queuedJobsMutex.unlock();
}

Job* JobSystem::ClaimJob(JobWorker* worker)
{
	m_queuedJobsMutex.lock();

	if (m_queuedJobs.empty())
	{
		m_queuedJobsMutex.unlock();
		return nullptr;
	}

	Job* job = m_queuedJobs.front();

	if ((job->m_Bitflags & worker->m_jobTypeBitflags) == 0)
	{
		m_queuedJobsMutex.unlock();
		return nullptr;
	}

	m_queuedJobs.pop_front();
	m_queuedJobsMutex.unlock();

	job->m_state = JobState::EXECUTING;

	m_executingJobsMutex.lock();
	m_executingJobs.push_back(job);
	m_executingJobsMutex.unlock();

	return job;
}

void JobSystem::CompleteJob(Job* jobToComplete)
{
	m_executingJobsMutex.lock();

	for (size_t i = 0; i < m_executingJobs.size(); i++)
	{
		if (m_executingJobs[i] == jobToComplete)
		{
			m_executingJobs.erase(m_executingJobs.begin() + i);
			m_executingJobsMutex.unlock();
			m_completedJobsMutex.lock();
			jobToComplete->m_state = JobState::COMPLETED;
			m_completedJobs.push_back(jobToComplete);
			m_completedJobsMutex.unlock();
			return;
		}
	}

	m_executingJobsMutex.unlock();
	return;
}

Job* JobSystem::RetrieveJob(Job* jobToRetrived)
{
	m_completedJobsMutex.lock();
	if (!m_completedJobs.empty())
	{
		if (jobToRetrived)
		{
			for (size_t i = 0; i < m_completedJobs.size(); i++)
			{
				if (m_completedJobs[i] == jobToRetrived)
				{
					m_completedJobs.erase(m_completedJobs.begin() + i);
					jobToRetrived->m_state = JobState::RETRIEVED;
					m_completedJobsMutex.unlock();
					return jobToRetrived;
				}
			}
		}
		else
		{
			Job* firstFinishedJob = m_completedJobs.front();
			m_completedJobs.pop_front();
			m_completedJobsMutex.unlock();
			return firstFinishedJob;
		}
	}

	m_completedJobsMutex.unlock();
	return nullptr;
}

size_t JobSystem::GetNumQueuedJobs() const
{
	m_queuedJobsMutex.lock();
	size_t numQueuedJob = m_queuedJobs.size();
	m_queuedJobsMutex.unlock();
	return numQueuedJob;
}

size_t JobSystem::GetNumCompletedJobs() const
{
	m_completedJobsMutex.lock();
	size_t numCompletedJob = m_completedJobs.size();
	m_completedJobsMutex.unlock();
	return numCompletedJob;
}

void JobSystem::ClearAllJobs()
{
	m_queuedJobsMutex.lock();
	m_executingJobsMutex.lock();
	m_completedJobsMutex.lock();

	for (size_t i = 0; i < m_queuedJobs.size(); i++)
	{
		delete m_queuedJobs[i];
	}
	for (size_t i = 0; i < m_executingJobs.size(); i++)
	{
		delete m_executingJobs[i];
	}
	for (size_t i = 0; i < m_completedJobs.size(); i++)
	{
		delete m_completedJobs[i];
	}

	m_queuedJobs.clear();
	m_executingJobs.clear();
	m_completedJobs.clear();

	m_queuedJobsMutex.unlock();
	m_executingJobsMutex.unlock();
	m_completedJobsMutex.unlock();
}

void JobSystem::SetWorkerThreadJobFlags(unsigned int bitflags, int num)
{
	int tracker = num;

	for (size_t i = 0; i < m_workers.size(); i++)
	{
		if (m_workers[i]->m_jobTypeBitflags == 1)
		{
			m_workers[i]->m_jobTypeBitflags = bitflags;
			tracker--;
			if (tracker <= 0)
			{
				return;
			}
		}
	}

}
