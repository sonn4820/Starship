#pragma once
#include <vector>
#include <deque>
#include <mutex>
#include <atomic>
#include <thread>

enum class JobState
{
	NEW,
	QUEUED,
	EXECUTING,
	COMPLETED,
	RETRIEVED
};
struct JobSystemConfig
{
	int m_numWorkers = -1;
};

struct Job
{
	Job() = default;
	virtual ~Job() = default;

	virtual void Execute() = 0;
	std::atomic<unsigned int> m_Bitflags = 1;
	std::atomic<JobState> m_state = JobState::NEW;
};


class JobWorker
{
	friend class JobSystem;

	JobWorker(int id, JobSystem* system);
	~JobWorker();

	void ThreadMain();

private:
	int m_id = -1;
	std::atomic<unsigned int> m_jobTypeBitflags = 1;
	JobSystem* m_system = nullptr;
	std::thread* m_thread = nullptr;
};

class JobSystem
{
	friend class JobWorker;

public:
	JobSystem(JobSystemConfig config);

	void Startup();
	void BeginFrame();
	void EndFrame();
	void Shutdown();

	JobSystemConfig m_config;

	void CreateWorkers(int num);
	void DestroyWorkers();
	void QueueJob(Job* jobToQueue);
	Job* ClaimJob(JobWorker* worker);
	void CompleteJob(Job* jobToComplete);
	Job* RetrieveJob(Job* jobToRetrived = nullptr);
	size_t GetNumQueuedJobs() const;
	size_t GetNumCompletedJobs() const;

	void ClearAllJobs();
	void SetWorkerThreadJobFlags(unsigned int bitflags, int num);

private:
	std::vector<JobWorker*> m_workers;
	std::deque<Job*> m_queuedJobs;
	std::deque<Job*> m_executingJobs;
	std::deque<Job*> m_completedJobs;
	mutable std::mutex m_queuedJobsMutex;
	mutable std::mutex m_executingJobsMutex;
	mutable std::mutex m_completedJobsMutex;
	std::atomic<bool> m_isShuttingDown = false;
};
