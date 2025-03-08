#![Wii Shoplifting Channel](http://transfer.archivete.am/9Qu6m/wiishop.png) <!-- ArchiveTeam rules! -->

This is a fork of larsenv's [WiiShopliftChannel](https://github.com/larsenv/WiiShopliftChannel). Commits that I have made are licensed under The Unlicense.

Writing instructions later.

<!-- Before we begin, I will say that I'm not responsible for what you do with this, this tool is a grey area.

This tool is intended to provide a way to download games from the Wii Shop directly onto your Wii, even after shutdown.

After the disaster that was Rii Shop where I was hit with a accusation that I used it to dox people, which was false (and I got harassment and some scary stuff happened as a result and it devastated me how the community reacted), people have tried making a Wii Shop revival themselves, however I found that all of them have failed to make a working shop revival. None of them worked...until now.

How does this work? You install a patched IOS56 for the Wii Shop Channel, install a ton of tickets, and then use this homebrew app to select what game you want from the Wii Shop Channel, and it launches the shop then navigates to the download page in the Wii Shop Channel.

# Instructions

1.	Perform a NAND backup prior to using this tool, as it can be potentially dangerous. Also, make sure you are using the latest version of the tool. Older versions have major issues that could mess up things.
2.	Download **WiiXplorer** (if you are using a Wii) and the **Wii Shoplift Channel** application.
3.	Put the app and the **WAD** folder included in the zip file on your SD/USB. (If using a Wii U, you do not need to copy the ticket folder)
4.	Launch your favorite **WAD Manager** and install the patched **IOS56** for your corresponding system.
5.	Get the title ID of the game you want to install from [http://github.com/larsenv/WiiShopliftChannel/blob/main/source/template.c#L46](here) and copy the ticket with that ID to the ticket folder on your SD (the filename is that ID without **00010001** at the start of the name). If using a Wii, follow the instructions below for **Installing Tickets (WiiXplorer)** or **Installing Tickets (FTP)**. Otherwise, if you are using a Wii U, follow the instructions below for **Installing Tickets (Wii U Only)**. (WiiXplorer cannot install tickets properly on a Wii U) ***We are working on a app update to copy the tickets automatically.***
7.	On the Wii Menu, you might get a message saying the Wii System Memory is full. If you do, then free up some space.
8.	Open **Wii Shoplift Channel** and select the game you want to download. The **Wii Shop Channel** will download the game, and it will appear on your **Wii Menu**. Also, if you enable the option to download directly to the SD, it may not work if the file size is too large

## Installing Tickets (WiiXplorer)

1. Open **WiiXplorer**. Go to **Start** > **Settings** > **Boot Settings** > **Enable NAND Write Access**. Press **Yes**, then **Accept**, then press **Back** until you return to the main menu.
2. Select the tickets you want to install. Press the **Plus Button**, then press **Copy**.
3. Navigate to **nand** -> **ticket** -> **00010001*. Press the **Plus Button** again, then press **Paste**. Wait until the process finishes. If you get a dialog asking if you want to replace files, select **No to All**.

## Installing Tickets (FTP)

1. Use **ftpii** to start up a FTP server.
2. Use a FTP client on another device and enter the IP and port of the FTP server.
3. Copy the tickets you want to your NAND folder -> **ticket** -> **00010001**. The NAND folder is called **isfs**. -->

# Wii Shop Error Codes

204036 - IOS isn't patched

204050 - Tickets not installed

205626 - Wii was region changed, either change your region to its prior setting or delete your Wii Shop account and try again

209601 - If you get this error you can ignore it
