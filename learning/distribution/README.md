# Distribution System

## Network reliablity

1. 如何在网络出现超时后，进行重试
2. 如何在重试时保证下游业务的幂等性
3. 如何在一个涉及链路较长的请求中找到问题
4. 如何观测整个系统的运行情况
5. 如何在复杂的网络链路中快速定位问题
6. 如何避免由于个别服务出错或超时带来的长尾效应

## Serveless

Serverless computing (or serverless for short), is an execution model where the cloud provider (AWS, Azure, or Google Cloud) is responsible for executing a piece of code by dynamically allocating the resources. And only charging for the amount of resources used to run the code. The code is typically run inside stateless containers that can be triggered by a variety of events including http requests, database events, queuing services, monitoring alerts, file uploads, scheduled events (cron jobs), etc. The code that is sent to the cloud provider for execution is usually in the form of a function. Hence serverless is sometimes referred to as “Functions as a Service” or “FaaS”.
