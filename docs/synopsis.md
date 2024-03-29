# Version description

**Creation time** : 2019-06-07 08:09

**Update time**   : 2019-09-04 12:15

## version v0.1

1. 该实例包含若干个initiator，一个router以及若干个memory作为target，其数量在common.h中声明为N_INIT和N_TARG;
2. initiator定义在initiator.h文件中，包含一个init_socket，该socket由simple_initiator_socket实现，并连接到router的target_socket;
3. router定义在router.h文件中，包含若干个socket，该socket由simple_target_socket实现；
4. memory定义在target.h文件中，扮演target的角色，包含1个targ_socket，该socket连接到router的init_socket；
5. 该版本能实现单向通信；

## version v0.2

将Router的socket全部改为simple_xxx_socket_tagged，并使用vector实现，保证和vgmn一致

## version v0.3

Router的定义和实现分离

## version v0.4

定义-实现分离

## version v0.5

加入central_buffer

## version v0.6

将SC_THREAD改为SC_METHOD

## version v0.7

1. 加入静态路由表
2. 修改payload字段

## Version v1.0

实现了动态路由，多init、多targ下测试系统正常、正确运行

## Version v1.1

1. 增加了对message和门铃事务包的描述,处理由DSP等部分实现
2. 增加了静态路由表,有读取文件建立静态路由表
3. 增加了soclib_payload_extension扩展负载,并确定payload必要字段和扩展字段
4. 修改了makefile文件,增加了对soclib相关头文件的支持
5. 增加了适配器,实现从tlm_socket向simple_socket接口的转化
6. 将调度算法抽离出来,不过需要配合centralized_buffer开辟的缓冲区共同使用
7. 调研了路由表的初始化,根据rapidIO实际情况进行初始化,思路已有,暂时剩下实现

## Version v0.15

1. 加入了系统初始化枚举,switch工作框架
2. 加入了对单播,多播,广播,以及系统初始化,维护包等的处理逻辑函数,等待后继实现
3. 对适配器进行了细节研究,初步确定适配器接口转化的可行性

## Version v0.16

实现了rapidio枚举初始化,形成相应路由表和拓扑结构

## Version v0.17

1. 对响应包同样采取前向传送路由方式,送到相应的返回端口,不采取缓存转发包的方式
2. 设计组播路由表为结构体链的形式,有利用动态扩展
3. 考虑4x和1x区别,4x是按时间先来先到转发,而1x是按优先级进行转发,所以在router中按时间顺序调度,而在target设置buffer缓冲对收到的包按pkt_id进行排序送到target中.

## Version v0.18

1. 这里将从端口进入对应缓冲改为从对应源设备id进入对应缓冲,后面改为source_id%5取余作为对应缓冲id,
因为考虑多个switch和设备id,在大型网络拓扑结构中
2. 考虑代码结构精简,许多复用代码抽出变成函数调用
3. 可以在路由模块中加入输出队列buffer,考虑存储效率等,暂不添加,可根据需要添加.

## Version v0.19

1. 改为四个init和四个target,同时测试buffer缓冲区每个buffer队列为3个大小时溢出情况处理
2. 对rapidio模块单独进行测试,查看测试结果
3. 大致提供了组播实现逻辑和框架,有待细化

## Version v0.20

1. 进行了多switch互连,测试系统初始化枚举,可以正常工作
2. 修改了调度算法pop_payload算法,对轮询逻辑重新进行了设置,使其正常工作

## Version v0.21

1. 完善了初始化算法,更加符合实际情况,同时添加了默认端口,即switch收到第一个初始化包时CAR[1]位置位为默认端口
2. 读取文件建立静态路由表时最好将文件名写成绝对路径,不然可能会报错
3. 调研确定响应函数最好通过nb_transport_bw函数前来实现,因为execLoop函数时周期性触发的,如果不通过nb_transport_bw函数,则payload.response响应可能无法及时让发送方知道

## Version v0.22

1. 将系统初始化维护包全部修改为存储转发实现
2. 确定错误维护机制为每个交换机维护自己转送到下一交换机的包,以此来收到相应的响应,该种情况就可以考虑payload.get_response_status函数实现
3. 确定switch每个时钟周期只弹出一个包
4. 通过frmae->Wdptr确定是写路由信息还是deviceid255对应的路由信息,通过payload->command确定下一次发送维护读还是维护写添加255对应路由信息

## Version v0.23

1. 实验了将routingtable作为模板参数放入router类,发现需要将router类的定义放在声明中,即.cpp文件内容应放在.h文件中或者
2. 可以在router.cpp文件后面显示加上template class Router<Routing_Table>
3. 在workspace文件夹下network文件夹中进行了简单的网络互传实例,通过ip地址结合端口号进行数据传输
4. 新设计了交换模型基类base_router.h和路由表基类base_routable,rapidio和以太网DSP内部交换等路由表均可以继承该基本路由表进行实现
5. 重新考虑了虚拟网络与真实网络互联的设计,通过建立客户端与服务器端的连接,并保持该连接不被kill,然后便可以实现客户端与服务器端的双向通信和数据传输

## Version v0.24

重新设计了base_router.h即交换模型基类，并且将RapidIO使用的router类继承base_router实现了路由表初始化建立过程，证明了设计基类的可行性，路由表基类设计，但暂未使用，直接在base_router的模板参数中使用Routing_Table

## Version v0.25

1. 删除代码中多余的注释部分
2. 实现了Routing_Table类暂时继承Base_Routing_Table类，作为初始化Router类时Base_Router的模板参数
