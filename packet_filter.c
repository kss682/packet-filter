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
};

SEC("xdp")
int drop_even_packet(struct xdp_md *ctx)
{
        void *data = (void *)(long)ctx->data;	
	void *data_end = (void *)(long)ctx->data_end;

	struct ethhdr *eth = data;
	if(eth + sizeof(*eth) > data_end)
		return XDP_DROP;

	struct iphdr *ip = data + sizeof(*eth);
	if(ip + sizeof(ip) > data_end)
		return XDP_DROP;

	if(ip->protocol != IPPROTO_UDP)
		return XDP_DROP;

	int ip_header_len = ip->ihl * 4;
	struct udphdr *udp = data + sizeof(*eth) + ip_header_len;
	
	if((void *)(udp + 1) > data_end)
		return XDP_DROP;
	
	struct payload *p = (void *)(udp + 1);
	if((void *)(p + 1) > data_end)
		return XDP_DROP;
	
	__u32 seq_id = bpf_ntohl(p->seq_id);
       if(seq_id&1)
       		return XDP_PASS;	       
	

	return XDP_DROP;
}
