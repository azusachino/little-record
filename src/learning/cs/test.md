# TEST

## Test Pattern

check here → [What's the difference between Red/Black deployment and Blue/Green Deployment?](https://stackoverflow.com/questions/45259589/whats-the-difference-between-red-black-deployment-and-blue-green-deployment)

### Blue/Green

The idea is to have two identical versions of your production environment, which we’ll call blue and green... Users of the system are routed to the green environment, which is the currently designated production. We want to release a new version of the application. So we deploy it to the blue environment... This does not in any way affect the operation of the green environment. We can run smoke tests against the blue environment to check it is working properly. When we’re ready, moving to the new version is as simple as changing the router configuration to point to the blue environment instead of the green environment. The blue environment thus becomes production. This switchover can typically be performed in much less than a second. If something goes wrong, we simply switch the router back to the green environment.'

### Red/Black

The Red version is live in production. You deploy the Black version to one or more servers. When the Black version is fully operational, you switch the router to direct all traffic to it (or you scale Red to 0 instances and Black to N). If anything goes wrong, you revert the operation. So, it's similar to blue-green deployment, but there's a **slight difference: in blue-green deployment, both versions may be getting requests at the same time temporarily, while in red-black only one of the versions is getting traffic at any point in time**. Here's some [corroboration](https://sweetibharti.wordpress.com/2016/11/11/red-black-deployment-on-aws/):

> At any time, only one of the environments is live, with the live environment serving all production traffic. For this example, Red is currently live and Black is idle (in this case we have kept the Black down-scaled to zero servers)...

Therefore, red-black is a specialization of blue-green. But red-black deployment is a newer term being used by Netflix, Istio, and other frameworks/platforms that support container orchestration. **The actual meaning can vary and many people are using "red-black" as another term for "blue-green", maybe just because their team colors are red and black**. :^)
