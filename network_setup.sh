#!/bin/bash

check_namespace(){
    local ns=$1
    if ! ip netns list | grep -q '$ns'; then
        echo "$ns namespace not created"
        exit 1
    fi
}

create_namespaces()
{
    ip netns add listener

    check_namespace "listener"
    echo "listener namesapce created"

    ip netns add talker

    check_namespace "talker"
    echo "talker namespace created"
}

setup_interfaces()
{
    ip link add veth-listener type veth peer name veth-talker

    if ! ip link | grep -q 'veth-listener@'; then
        echo "veth-listener interface not created"
        exit 1
    fi
    echo "veth-listener interface created"

    if ! ip link | grep -q "veth-talker@"; then
        echo "veth-talker interface not created"
        exit 1
    fi
    echo "veth-talker interface created"

    # add the interface to their corresponding namespaces
    ip link set veth-listener netns listener
    ip link set veth-talker netns talker

    ip netns exec listener ip address add 10.0.0.1/24 dev veth-listener
    ip netns exec listener ip link set veth-listener up
    ip netns exec talker ip link set veth-talker up

    ip netns exec talker ip address add 10.0.0.2/24 dev veth-talker
}


create_namespaces

setup_interfaces