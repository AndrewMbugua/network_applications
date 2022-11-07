bool bgp_4_need_mpbgp(struct bgp_4_neighbor_config *n)
{
    if(n->afi_safi == NULL)
        return false;
    struct bgp_4_afi_safi *afi_safi = n->afi_safi;
    while(afi_safi)
    {
        if(afi_safi->afi != IPV4_AFI && afi_safi->safi != UNICAST_SAFI)
            return true;
        afi_safi = afi_safi->next;
    }
    return false;
}

static inline void bgp_4_append_cap_header(__u8 *buffer, __u16 *end, __u8 cap_code, __u8 cap_len)
{
    buffer[(*end)++] = BGP_CAP_PARAM_TYPE;
    buffer[(*end)++] = cap_len+2;
    buffer[(*end)++] = cap_code;
    buffer[(*end)++] = cap_len;
}

static inline void bgp_4_append_mpbgp_cap_data(__u8 *buffer, __u16 *end, __u16 afi, __u8 safi)
{
    APPEND_16_PTR(buffer, end, __bswap_16(afi));
    buffer[(*end)++] = 0;
    buffer[(*end)++] = safi;
}

static inline void bgp_4_append_mpbgp_cap(__u8 *buffer, __u16 *end, struct bgp_4_afi_safi *afi_safi)
{
    while(afi_safi)
    {
        bgp_4_append_cap_header (buffer, end, BGP_MPBGP_CAP_T, BGP_MPBGP_CAP_LEN);
        bgp_4_append_mpbgp_cap_data(buffer, end, afi_safi->afi, afi_safi->safi);
        afi_safi = afi_safi->next;
    }
}

static inline void bgp_4_append_as4_cap_data(__u8 *buffer, __u16 *end, __u32 asn)
{
    APPEND_32_PTR(buffer, end, __bswap_32(asn));
}

void bgp_4_append_as4_cap(__u8 *buffer, __u16 *end, __u32 asn)
{
    bgp_4_append_cap_header(buffer, end, BGP_AS4_CAP_T, BGP_AS4_CAP_LEN);
    bgp_4_append_as4_cap_data(buffer, end, asn);
}

int bgp_4_queue_initial_open(struct bgp_4_neighbor *neigh)
{
    if(BUFFER_SIZE - neigh->tcp_ses->send_end < 4096)
        return -1;          // No buffer size to write the packet
    rte_spinlock_lock (&neigh->tcp_ses->session_write_lock);
    struct tcp_session *s = neigh->tcp_ses;
    __u8 *buffer = &s->sendbuf[s->send_end];
    struct bgp_4_header *head = (void *)buffer;
    struct bgp_4_open_header *open = (void *)(head+1);
    memset(head, 0, sizeof(*head));
    memset(open, 0, sizeof(*open));
    *(__u64*)&head->marker[0] = ~(__u64)(0);
    *(__u64*)&head->marker[8] = ~(__u64)(0);
    head->type = BGP_OPEN_T;
    open->version = 4;
    if(neigh->conf->local_asn < 65535)
        open->as16 = __bswap_16(neigh->conf->local_asn);
    else
        open->as16 = __bswap_16(BGP_AS_TRANS);
    open->holdtime = neigh->conf->keepalive_time;
    open->bgp_id = __bswap_32(neigh->local_router_id);
    neigh->tcp_ses->send_end += (sizeof(*head) + sizeof(*open));    // Position the end so we can easily add parameters
    if(bgp_4_need_mpbgp(neigh->conf)) {
        bgp_4_append_mpbgp_cap(buffer, &s->send_end, neigh->conf->afi_safi);
    }
    bgp_4_append_as4_cap (buffer, &s->send_end, neigh->conf->local_asn);
    open->param_len = (__u8)((buffer+s->send_end) - (__u8*)(open+1));
    rte_spinlock_unlock (&s->session_write_lock);
    return 0;
}
