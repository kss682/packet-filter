#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_endian.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/in.h>
#include <linux/types.h>
#include <linux/udp.h>

char _license[] SEC("license") = "GPL";

struct payload{
	__u32 seq_id;
}__attribute__((packed));

SEC("xdp")
int drop_even_packet(struct xdp_md *ctx)
{
        void *data = (void *)(long)ctx->data;	
	void *data_end = (void *)(long)ctx->data_end;

	struct ethhdr *eth = data;
	if((void *)(eth + 1) > data_end)
		return XDP_DROP;

	bpf_printk("Ethernet header correct");
	bpf_printk("Ether protocol: %u", eth->
	struct iphdr *ip = data + sizeof(*eth);
	if((void *)(ip + 1) > data_end)
		return XDP_DROP;

	bpf_printk("IP header correct");
	bpf_printk("Using proto %u", ip->protocol);
	/*if(ip->protocol != IPPROTO_UDP)
		return XDP_DROP;
	*/
	int ip_header_len = ip->ihl * 4;
	struct udphdr *udp = data + sizeof(*eth) + ip_header_len;
	
	if((void *)(udp + 1) > data_end)
		return XDP_DROP;
	bpf_printk("UDP header correct");

	struct payload *p = (void *)(udp + 1);
	if((void *)(p + 1) > data_end)
		return XDP_DROP;
 	
       	bpf_printk("Packet with id %u", p->seq_id);
	__u32 seq_id = bpf_ntohl(p->seq_id);
	if(seq_id&1)
       		return XDP_PASS;	       
	

	return XDP_DROP;
}
