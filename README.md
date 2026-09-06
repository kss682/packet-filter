# Packet Filter
A sample repository to learn more about BPF technology. 

Ubuntu VM is used as a host machine to test scenario.
We create two virtual interfaces in the namespace `listener` and `talker` respectively. 
The `talker` namespace has a `UDP` packet generator, which generates sequential packet IDs. 
On the listener side, we attach a BPF program on the virtual interface to filter even ID packets from `talker`.