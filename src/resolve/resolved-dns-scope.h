/*-*- Mode: C; c-basic-offset: 8; indent-tabs-mode: nil -*-*/

#pragma once

/***
  This file is part of systemd.

  Copyright 2014 Lennart Poettering

  systemd is free software; you can redistribute it and/or modify it
  under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation; either version 2.1 of the License, or
  (at your option) any later version.

  systemd is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with systemd; If not, see <http://www.gnu.org/licenses/>.
***/

#include "list.h"

typedef struct DnsScope DnsScope;

#include "resolved.h"
#include "resolved-link.h"
#include "resolved-dns-server.h"
#include "resolved-dns-packet.h"
#include "resolved-dns-query.h"

typedef enum DnsScopeType {
        DNS_SCOPE_DNS,
        DNS_SCOPE_MDNS,
} DnsScopeType;

typedef enum DnsScopeMatch {
        DNS_SCOPE_NO,
        DNS_SCOPE_MAYBE,
        DNS_SCOPE_YES,
        _DNS_SCOPE_MATCH_MAX,
        _DNS_SCOPE_INVALID = -1
} DnsScopeMatch;

struct DnsScope {
        Manager *manager;

        DnsScopeType type;
        unsigned char family;

        Link *link;

        char **domains;

        LIST_HEAD(DnsQueryTransaction, transactions);

        LIST_FIELDS(DnsScope, scopes);
};

int dns_scope_new(Manager *m, DnsScope **ret, DnsScopeType t);
DnsScope* dns_scope_free(DnsScope *s);

int dns_scope_send(DnsScope *s, DnsPacket *p);
DnsScopeMatch dns_scope_test(DnsScope *s, const char *domain);

DnsServer *dns_scope_get_server(DnsScope *s);
void dns_scope_next_dns_server(DnsScope *s);