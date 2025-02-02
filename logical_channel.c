#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void get_user_input(char *ssid, char *ip_address, char *netmask, char *gateway, char *channel) {
    printf("Enter the SSID (Network Name):");
    fgets(ssid, 100, stdin);
    ssid[strcspn(ssid, "\n")] = 0;  // Remove the newline character
    
    printf("Enter the IP address:");
    fgets(ip_address, 100, stdin);
    ip_address[strcspn(ip_address, "\n")] = 0;

    printf("Enter the network netmask (e.g., 255.255.255.0):");
    fgets(netmask, 100, stdin);
    netmask[strcspn(netmask, "\n")] = 0;

    printf("Enter the gateway IP address (optional, press Enter to skip): ");
    fgets(gateway, 100, stdin);
    gateway[strcspn(gateway, "\n")] = 0;  // Remove newline from gateway input

    printf("Enter the channel number (1-11 for 2.4GHz, 36-165 for 5GHz):");
    fgets(channel, 100, stdin);
    channel[strcspn(channel, "\n")] = 0;
}

void configure_ad_hoc_network(const char *ssid, const char *ip_address, const char *netmask, const char *gateway, const char *channel) {
    const char *interface = "wlan0";  // You may need to change this based on the OS and hardware
    char command[512];

    // Disable Wi-Fi before configuring
    snprintf(command, sizeof(command), "sudo ifconfig %s down", interface);
    system(command);

    // Set up ad-hoc mode
    snprintf(command, sizeof(command), "sudo iw dev %s set type ibss", interface);
    system(command);

    // Configure SSID
    snprintf(command, sizeof(command), "sudo iw dev %s ibss join %s %s", interface, ssid, channel);
    system(command);

    // Assign IP address
    snprintf(command, sizeof(command), "sudo ifconfig %s %s netmask %s", interface, ip_address, netmask);
    system(command);

    if (strlen(gateway) > 0) {
        // Set up the gateway if provided
        snprintf(command, sizeof(command), "sudo route add default gw %s %s", gateway, interface);
        system(command);
    }

    // Enable the network interface
    snprintf(command, sizeof(command), "sudo ifconfig %s up", interface);
    system(command);

    printf("Ad-hoc network '%s' configured on channel %s with IP %s.\n", ssid, channel, ip_address);
}

int main() {
    char ssid[100], ip_address[100], netmask[100], gateway[100], channel[100];

    printf("======================= Ad-hoc Network Configuration Tool=======================\n\n");

    // Get user input for network details
    get_user_input(ssid, ip_address, netmask, gateway, channel);

    // Configure the ad-hoc network
    configure_ad_hoc_network(ssid, ip_address, netmask, gateway, channel);

    printf("Ad-hoc network setup complete.\n");

    return 0;
}
