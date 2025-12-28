# OSIFS (*OS-Independent File Sharing*): seamless data transfer between non-mobile devices reagrdless of their proprietary operating systems

## Why is it, that in 2025, we are still sending things between computers via email?

I wanted to solve this issue, and I looked no further than the most convenient, lucrative, and effective file transfer service ever - **AirDrop**. This service effectively created and maintained Apple's dominant ecosystem, and is extremely helpful and versatile. Although I couldn't implement this to include mobile as well *even though if I could, I'd probably be a millionaire*, it is aimed at seamless network transfer to hosts on the same LAN.

I took a Computer Networking class at Stony Brook last semester, and thought I should practice some C programming to build this functionality. I needed to create a simple protocol, broadcasting, and daemon to enable this feature.

>It is currently a work in progress, but I've outlined the project (*That I'm calling **OSIFS***) below!

## How it works: Key Product Design Overview

### Motivation

As is well known, Apple products' success is largely attributable to its ease of use and the interoperability created by its **ecosystem**. Airpods automatically and seamlessly switch between devices without need for manual Bluetooth connection management, notifications from your phone automatically show up on your Mac, and, more importantly, files are transferred seamlessly between devices via AirDrop. This makes it super easy to snap a picture, scan a file, and send it over to your laptop to process, submit, or edit.

As a computer science student and tech enthusiast, I have many devices of different operating systems. I have a Linux Laptop, a MacBook Air, and a Windows PC, along with an iPhone.

Transferring data between the Apple systems is super easy - you just click AirDrop, select a file
and then boom - it shows up on the other host.

However, transferring data between OS's usually involves sending an email to yourself. Say I wanted to send over a scanned PDF document from my phone to my Linux laptop. Currently, I, as well as most other people, do so by sending it to ourselves over email. Likewise, sending things between computers that are not on the same Apple ecosystem is also difficult and involves the same treacherous process of using the fabled, yet scary *Gmail*.

> Note: It is interesting that there isn't even any accepting that needs to occur between Mac and iPhone on the same AppleID - it happens seamlessly and automatically.

## Issues with sending information over email

I see two main problems with the traditional inter-OS transfer approach:

1. **Annoyance** - it is very annoying, clunky, and disorganized to send an email to yourself. You have to think of the title to remember it by, you must put in some body, attach a file, and *ughh.... Soooooo long!!* What if you could just select which computer to send it to, and send what you want to send?? C'mon guys, it's 2025!

2. **Security** - I personally share an email with my friend to share some accounts and subscriptions. I suspect that many people do the same. This means that sensitive information cannot be sent, as it would be available to everyone logged into the account. Even if this issue doesn't affect you, if, for some reason, someone did receive access to your account in the future, they would have accesse to all those unnamed emails of scans of IDs and passports and SSN's and everything like that.

## Solution

So, why don't we try to emulate features of AirDrop, bypassing the clunky, insecure, inefficient medium of self-emails, specifically for computer systems?

> Note: Although this feature would be most useful for mobile devices, I wanted to aim to solve the problem of transferring files between computers first, as I'm quite unfamiliar with mobile programming.

## Study: Key features of AirDrop to emulate

1. **AirDrop is close range and wireless.** AirDrop does not work for remote devices on the same AppleID. This inherently limits the scope of its purpose, which I'd like to define as **Quick and easy file transfer between nearby devices**. The fact that it is wireless is essential to its ease of use, as without this feature, manual flash drive transfer would be just as efficient

2. **AirDrop shows you which devices you can send to.** This is extremely convenient for defining the recipient of your files: everyone who is available is shown and everyone who isn't is inaccessible. This must be done dynamically because you can add devices, subscribe new devices to the protocol, and, most importantly, toggle participation for security purposes

3. **AirDrop is toggleable.** You can turn AirDrop off to prevent others from requesting to send you files. With AirDrop specifically, you can also set preferences to the devices you'd like to be discoverable to - Contacts only, Everyone, or Off. Since the notion of contacts is not really useful, **OSIFS** will have two simple modes: **Everyone**: discoverable by everyone on the same LAN, and **No one**: refusing to participate in the protocol: undiscoverable.

## OSIFS Protocol Definition

The OSIFS protocol will have the following types of messages:

| Type | Purpose | Contains | Sender | Recipient |  
| :--- | :------ | :------- | :----- | :-------- |
| `M_BROADCAST` | Initial packet that is sent to the entire LAN to notify every device on the LAN that a new device is connected to OSFIS | Contains the IP address of the new device that joined, and its PORT number | New ==^1^== device on the network | Everyone ==^2^== on the LAN |
| `M_IDENTIFY` | Upon receiving the `M_BROADCAST` message, every other device that uses the **OSIFS** protocol will send an Identifying message to let the new device know its identifying information like its IP and PORT | Contains IP address of the device that received the `M_BROADCAST` packet. | Recipient of the `M_BROADCAST` packet | Sender of the `M_BROADCAST` packet |
| `M_SEND` | Sends the actual file via TCP ==^3^== | File, its metadata, and its OSIFS FileID ==^4^== | Whoever has the data | Desired recipient of the data |
| `M_ACK` | Lets the sender of the data know that the file has been delivered and interpreted succsessfully | The OSIFS FileID | Whoever received the data | Whoever sent the data originally |
| `M_NACK` | Lets the sender of the data know that the file has **not** been delivered or interpreted correctly | The OSIFS FileID of the incorrect file | Whoever received the incorrect data | Wheover sent the incorrect data |

> ==1.== New means that the device can be a first-time user (just donwloaded the protocol), or that it could be toggling on the protocol after having used it before, thus becoming newly discoverable.

> ==2.== Everyone on the LAN truly does mean *everyone*: the discovery packet is broadcasted via **UDP** to the IP Address 255.255.255.255, meaning that every machine recieves it.

> ==3.== The discovery process happens over UDP because it must be broadcast and is therefore connectionless, while the actual transfer and connection established must be reliable, hence we use TCP

> ==4.== OSIFS FileID is a non-negative number that can be used to identify which file has been properly sent and received. It is more efficient than using filenames to identify data

## In depth plan of implementation

We first must establish a **Well-Known OSIFS Port**, which I arbitrarily chose to be `8000`. Likewise, we must establish a TCP `listening` port for actual connections that are established.
==For the remainder of the documentation, I'll refer to this port as `UPD PORT`.== This will be the main UDP port that listens and sends broadcast packets to identify itself on the protocol.

1. On start of the protocol, a device sends an `M_BROADCAST` packet to IP address `255.255.255.255` containing its identifying `IP` address.

2. It then begins listening[^1] on the `UDP PORT` for any broadcast packets. 

[^1]: listening in this sense does not refer to the `listen` system call that is used for TCP connections. Instead, my use of the word listening in this context simply means that the host calls on `recv()` with the nonblocking function - processing packets that pass in on this port. For future note, I'll format the system call like follows: "`listen`" is the syscall and "listen" is the informal definition above