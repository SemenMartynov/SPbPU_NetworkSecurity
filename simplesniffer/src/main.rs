use pdu::*;
use rawsock::open_best_library;

fn main() {
    println!("Opening packet capturing library");
    let lib = open_best_library().expect("Could not open any packet capturing library");
    println!("Library opened, version is {}", lib.version());
    let interf_name = "wlp3s0"; //replace with whatever is available on your platform
    println!("Opening the {} interface", interf_name);
    let mut interf = lib
        .open_interface(&interf_name)
        .expect("Could not open network interface");
    println!("Interface opened, data link: {}", interf.data_link());

    //receive some packets.
    println!("Receiving 5 packets:");
    for _ in 0..5 {
        let packet = interf.receive().expect("Could not receive packet");
        println!("Received packet: {:02x?}", packet.as_ref());

        match EthernetPdu::new(&packet) {
            Ok(ethernet_pdu) => {
                println!(
                    "[ethernet] destination_address: {:x?}",
                    ethernet_pdu.destination_address().as_ref()
                );
                println!(
                    "[ethernet] source_address: {:x?}",
                    ethernet_pdu.source_address().as_ref()
                );
                println!(
                    "[ethernet] ethertype: 0x{:04x}",
                    ethernet_pdu.ethertype()
                );
                if let Some(vlan) = ethernet_pdu.vlan() {
                    println!(
                        "[ethernet] vlan: 0x{:04x}",
                        vlan
                    );
                }
                match ethernet_pdu.inner() {
                    Ok(Ethernet::Arp(arp_pdu)) => {
                        println!(
                            "[ARP] operation code: 0x{:02x}",
                            arp_pdu.opcode()
                        );
                        println!(
                            "[ARP] Sender hardware address: {:x?}",
                            arp_pdu.sender_hardware_address().as_ref()
                        );
                        println!(
                            "[ARP] Sender protocol address: {:x?}",
                            arp_pdu.sender_protocol_address().as_ref()
                        );
                        println!(
                            "[ARP] Target hardware address: {:x?}",
                            arp_pdu.target_hardware_address().as_ref()
                        );
                        println!(
                            "[ARP] Target protocol address: {:x?}",
                            arp_pdu.target_protocol_address().as_ref()
                        );
                    }
                    Ok(Ethernet::Ipv4(ipv4_pdu)) => {
                        println!(
                            "[ipv4] source_address: {:x?}",
                            ipv4_pdu.source_address().as_ref()
                        );
                        println!(
                            "[ipv4] destination_address: {:x?}",
                            ipv4_pdu.destination_address().as_ref()
                        );
                        println!(
                            "[ipv4] protocol: 0x{:02x}",
                            ipv4_pdu.protocol()
                        );
                        match ipv4_pdu.inner() {
                            Ok(Ipv4::Tcp(tcp_pdu)) => {
                                println!(
                                    "[TCP] Source port: {:x?}",
                                    tcp_pdu.source_port()
                                );
                                println!(
                                    "[TCP] Destination port : {:x?}",
                                    tcp_pdu.destination_port()
                                );
                                println!(
                                    "[TCP] Sequence number: {:x?}",
                                    tcp_pdu.sequence_number()
                                );
                            }
                            Ok(Ipv4::Udp(udp_pdu)) => {
                                println!(
                                    "[UDP] Source port: {:x?}",
                                    udp_pdu.source_port()
                                );
                                println!(
                                    "[UDP] Destination port : {:x?}",
                                    udp_pdu.destination_port()
                                );
                                println!(
                                    "[UDP] Length: {:x?}",
                                    udp_pdu.length()
                                );
                            }
                            Ok(Ipv4::Icmp(icmp_pdu)) => {
                                println!(
                                    "[ICMP] Message code: 0x{:02x?}",
                                    icmp_pdu.message_code()
                                );
                                println!(
                                    "[ICMP] Message type : 0x{:02x?}",
                                    icmp_pdu.message_type()
                                );
                                println!(
                                    "[ICMP] Checksum: {:x?}",
                                    icmp_pdu.checksum()
                                );
                            }
                            Ok(other) => {
                                println!(
                                    "Other (unexpected) packet {:?}",
                                    other
                                );
                            }
                            Err(e) => {
                                panic!("Ipv4::inner() parser failure: {:?}", e);
                            }
                        }
                    }
                    Ok(Ethernet::Ipv6(ipv6_pdu)) => {
                        println!(
                            "[ipv6] source_address: {:x?}",
                            ipv6_pdu.source_address().as_ref()
                        );
                        println!(
                            "[ipv6] destination_address: {:x?}",
                            ipv6_pdu.destination_address().as_ref()
                        );
                        println!(
                            "[ipv6] protocol: 0x{:02x}",
                            ipv6_pdu.computed_protocol()
                        );
                        match ipv6_pdu.inner() {
                            Ok(Ipv6::Tcp(tcp_pdu)) => {
                                println!(
                                    "[TCP] Source port: {:x?}",
                                    tcp_pdu.source_port()
                                );
                                println!(
                                    "[TCP] Destination port : {:x?}",
                                    tcp_pdu.destination_port()
                                );
                                println!(
                                    "[TCP] Sequence number: {:x?}",
                                    tcp_pdu.sequence_number()
                                );
                            }
                            Ok(Ipv6::Udp(udp_pdu)) => {
                                println!(
                                    "[UDP] Source port: {:x?}",
                                    udp_pdu.source_port()
                                );
                                println!(
                                    "[UDP] Destination port : {:x?}",
                                    udp_pdu.destination_port()
                                );
                                println!(
                                    "[UDP] Length: {:x?}",
                                    udp_pdu.length()
                                );
                            }
                            Ok(Ipv6::Icmp(icmp_pdu)) => {
                                println!(
                                    "[ICMP] Message code: 0x{:02x?}",
                                    icmp_pdu.message_code()
                                );
                                println!(
                                    "[ICMP] Message type : 0x{:02x?}",
                                    icmp_pdu.message_type()
                                );
                                println!(
                                    "[ICMP] Checksum: {:x?}",
                                    icmp_pdu.checksum()
                                );
                            }
                            Ok(other) => {
                                println!(
                                    "Other (unexpected) packet {:?}",
                                    other
                                );
                            }
                            Err(e) => {
                                panic!("Ipv4::inner() parser failure: {:?}", e);
                            }
                        }
                    }
                    Ok(other) => {
                        println!(
                            "Other (unexpected) protocol {:?}",
                            other
                        );
                    }
                    Err(e) => {
                        panic!("EthernetPdu::inner() parser failure: {:?}", e);
                    }
                }
            }
            Err(e) => {
                panic!("EthernetPdu::new() parser failure: {:?}", e);
            }
        }
    }
}
