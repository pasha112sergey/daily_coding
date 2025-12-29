# OSIFS (*OS-Independent File Sharing*): seamless data transfer between non-mobile devices regardless of their proprietary operating systems

## Why is it, that in 2025, we are still sending things between computers via email?

I wanted to solve this issue, and I looked no further than the most convenient, lucrative, and effective file transfer service ever - **AirDrop**. This service effectively created and maintained Apple's dominant ecosystem, and is extremely helpful and versatile. Although I couldn't implement this to include mobile as well *even though if I could, I'd probably be a millionaire*, it is aimed at seamless network transfer to hosts on the same LAN.

I took a Computer Networking class at Stony Brook last semester, and thought I should practice some C programming to build this functionality. I needed to create a simple protocol, broadcasting, and daemon to enable this feature.

>It is currently a work in progress, but I've outlined the project (*That I'm calling **OSIFS***) below!

## How it works: Key Product Design

### Motivation

As is well known, Apple products' success is largely attributable to its ease of use and the interoperability created by its **ecosystem**. AirPods automatically and seamlessly switch between devices without need for manual Bluetooth connection management, notifications from your phone automatically show up on your Mac, and, more importantly, files are transferred seamlessly between devices via AirDrop. This makes it super easy to snap a picture, scan a file, and send it over to your laptop to process, submit, or edit.

As a computer science student and tech enthusiast, I have many devices of different operating systems. I have a Linux Laptop, a MacBook Air, and a Windows PC, along with an iPhone.

Transferring data between the Apple systems is super easy - you just click AirDrop, select a file
and then boom - it shows up on the other host.

However, transferring data between OS's usually involves sending an email to yourself. Say I wanted to send over a scanned PDF document from my phone to my Linux laptop. Currently, I, as well as most other people, do so by sending it to ourselves over email. Likewise, sending things between computers that are not on the same Apple ecosystem is also difficult and involves the same treacherous process of using the fabled, yet scary *Gmail*.

> ***Note:** It is interesting that there isn't even any accepting that needs to occur between Mac and iPhone on the same AppleID - it happens seamlessly and automatically.*

## Issues with sending information over email

I see two main problems with the traditional inter-OS transfer approach:

1. **Annoyance** - it is very annoying, clunky, and disorganized to send an email to yourself. You have to think of the title to remember it by, you must put in some body, attach a file, and *ughh.... Soooooo long!!* What if you could just select which computer to send it to, and send what you want to send?? C'mon guys, it's 2025!

2. **Security** - I personally share an email with my friend to share some accounts and subscriptions. I suspect that many people do the same. This means that sensitive information cannot be sent, as it would be available to everyone logged into the account. Even if this issue doesn't affect you, if, for some reason, someone did receive access to your account in the future, they would have access to all those unnamed emails of scans of IDs and passports and SSN's and everything like that.

## Solution

So, why don't we try to emulate features of AirDrop, bypassing the clunky, insecure, inefficient medium of self-emails, specifically for computer systems?

> Note: Although this feature would be most useful for mobile devices, I wanted to aim to solve the problem of transferring files between computers first, as I'm quite unfamiliar with mobile programming.

## Study: Key features of AirDrop to emulate

1. **AirDrop is close range and wireless.** AirDrop does not work for remote devices on the same AppleID. This inherently limits the scope of its purpose, which I'd like to define as **Quick and easy file transfer between nearby devices**. The fact that it is wireless is essential to its ease of use, as without this feature, manual flash drive transfer would be just as efficient

2. **AirDrop shows you which devices you can send to.** This is extremely convenient for defining the recipient of your files: everyone who is available is shown and everyone who isn't is inaccessible. This must be done dynamically because you can add devices, subscribe new devices to the protocol, and, most importantly, toggle participation for security purposes

3. **AirDrop is toggleable.** You can turn AirDrop off to prevent others from requesting to send you files. With AirDrop specifically, you can also set preferences to the devices you'd like to be discoverable to - Contacts only, Everyone, or Off. Since the notion of contacts is not really useful, **OSIFS** will have two simple modes: **Everyone**: discoverable by everyone on the same LAN, and **No one**: refusing to participate in the protocol: undiscoverable.

> ***Note:** AirDrop also benefits from a lack of constraint on file size. I would like to replicate this to the best of my ability. Solutions like background file splitting and reconstruction for large files can be conceived, but currently it would do more harm than good. My top priority at this point is to build the MVP as fast as possible to prove a concept. I shall do so by setting a large `MAX_PAYLOAD` size of **~100 MB** (1e+8 bytes), which should be more than enough for common data like photos and files.*

## OSIFS Protocol Definition

The OSIFS protocol will have the following types of messages:

| Type | Purpose | Contains | Sender | Recipient |  
| :--- | :------ | :------- | :----- | :-------- |
| `M_BROADCAST` | Initial packet that is sent to the entire LAN to notify every device on the LAN that a new device is connected to OSFIS | Contains the IP address of the new device that joined, and its PORT number | New ==^1^== device on the network | Everyone ==^2^== on the LAN |
| `M_IDENTIFY` | Upon receiving the `M_BROADCAST` message, every other device that uses the **OSIFS** protocol will send an Identifying message to let the new device know its identifying information like its IP and PORT | Contains IP address of the device that received the `M_BROADCAST` packet. | Recipient of the `M_BROADCAST` packet | Sender of the `M_BROADCAST` packet |
| `M_SEND` | Sends the actual file via TCP ==^3^== | File, its metadata, and its OSIFS FileID ==^4^== | Whoever has the data | Desired recipient of the data |
| `M_ACK` | Lets the sender of the data know that the file has been delivered and interpreted successfully | The OSIFS FileID | Whoever received the data | Whoever sent the data originally |
| `M_NACK` | Lets the sender of the data know that the file has **not** been delivered or interpreted correctly. Also sent when the receiver declines | The OSIFS FileID of the incorrect file and the reason for rejection. | Whoever received the incorrect data | Whoever sent the incorrect data |

> ==1.== New means that the device can be a first-time user (just downloaded the protocol), or that it could be toggling on the protocol after having used it before, thus becoming newly discoverable.

> ==2.== Everyone on the LAN truly does mean *everyone*: the discovery packet is broadcasted via **UDP** to the IP Address 255.255.255.255, meaning that every machine receives it.

> ==3.== The discovery process happens over UDP because it must be broadcast and is therefore connectionless, while the actual transfer and connection established must be reliable, hence we use TCP

> ==4.== OSIFS FileID is a non-negative number that can be used to identify which file has been properly sent and received. It is more efficient than using filenames to identify data

## In depth plan of implementation

There are 2 main roles that each host that uses **OSIFS** will play: that of the *sender* and *receiver*.

I wanted to separate the functionality neatly. For this, I will use threads! I chose to go with threads instead of processes because it will be necessary to communicate information about the available **OSIFS** hosts.

>***Note:** I'm planning to store this information in an array, but more on that later*

So, we will have 2 threads: the Sender, and the Receiver.[^1] Their responsibilities are outlined below:

|Sender | Receiver |
| :---- | :------- |
| Presents available host destinations | Handles `M_BROADCAST` packets on the `UDP` Port [^2] |
| Processes user input to select the destination | Handles sending `M_IDENTIFY` packets upon receiving `M_BROADCAST` packet |
| Takes in user input that specifies the file to send | Handles listening for connection requests on the `TCP` port |
| Sets up TCP connection creation with desired destination host | Handles managing `TCP` connections |
| Sends the file by sending the `M_SEND` packet containing data | Handles sending `M_ACK` AND `M_NACK` packets |

Since the **Sender** needs information about available hosts from the **Receiver**, we must establish a shared data structure: `available_hosts`. This array will hold all the current available hosts, and will be populated by the receiver gets the `M_IDENTIFY` packets. Since concurrent access may occur, we should protect it with a mutex.

## Presentation

Now let's move our discussion to the overall presentation of this service. Taking a page out of AirDrop's book, it is a simple interface where you can just click a host, select the file to transfer, and wait for confirmation. This is something I'd like to emulate.

However, a traditional, simple commandline program poses a couple of challenges to this implementation. First, there needs to be a sensible notion of different screens. For instance, the screen that specifies the host is logically distinct from the screen that specifies file to send. Likewise, what happens if the user is browsing through their file system to send a file to a host, while someone else is requesting to send a file to them? All of these problems necessitate a clear visual interface that is real-time responsive, reinforcing a need for a concurrent approach.

This must be done seamlessly, but I still want to be able to call this from the commandline. Likewise, I want to learn how to make a cool TUI, so I shall pick a library that allows me to do that.

After doing some minimal online, it seems like the `ncurses` library is optimal for TUI programming with **C**. So, I'll try to use it, but as the project progresses I might change my implementation, so stay tuned for updates.

> ***Note:** The presentation will be the last step in the project, as the first priority is to build the MVP to demonstrate file transferring functionality, and its viability as a useful service.*

[^1]: Although they are called Sender and Receiver, both will be performing some function pertaining to sending and receiving packets. The distinction lies in whether the user *sending* a file vs *receiving* a file

[^2]: The receiver manages 2 "OSIFS well-known" ports : the `UDP` broadcast port (8000), and the `TCP` listening port (8001)
