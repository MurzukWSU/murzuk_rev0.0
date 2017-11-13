
Texas Instruments, Inc.

P a c k e t  s n i f f e r  Release Notes

-------------------------------------------------------------------------------
-------------------------------------------------------------------------------

-------------------------------------------------------------------------------
N O T E!
To ensure that everything is cleaned up from the startup menu, it is recomended to
uninstall the old version before installing the new version.
-------------------------------------------------------------------------------

-------------------------------------------------------------------------------
Version 2.18.0.0
June 25. 2014
-------------------------------------------------------------------------------
- Support for CC2544 Dongle.
- Updated FW for SmartRF05EB & TrxEB. Faster packet handling for CC2520.
- Improved handling of temporary file. More robust cleanup.
- Processing of timestamp improved.
  
-------------------------------------------------------------------------------
Version 2.17.1.2
November 26. 2013
-------------------------------------------------------------------------------
- Sniffer FW for CC2540EM and CC2540 USB Dongle updated. 
-------------------------------------------------------------------------------
Version 2.17.1.1
October 24. 2013
-------------------------------------------------------------------------------
- Added feature when reading .psd file that will analyze the data in the .psd 
  file and try to determine the format. In most cases it will recognize the new 
  format and don't have to ask the user for information.
-------------------------------------------------------------------------------
Version 2.17.1.0
October 18. 2013
-------------------------------------------------------------------------------
- Support for reading of .psd file with old format.
- BLE: Added retry mechanism for L2CAP decryption in case sniffer loses 
       sync with OTA packet count.
-------------------------------------------------------------------------------
Version 2.17.0
July 25. 2013
-------------------------------------------------------------------------------
- Support for TrxEb + CC1200EM.
- Support long packets for CC1200 and Generic protocol.
- Input field for Long Term Key for BLE protocol.
- Fixed problem with RSSI value when reading data from .psd file.
- Fixed problem with Sniffer hanging if first packet is returned with an error.
- BLE: ATT protocol handling updated to Core V4 spec.
- BLE: SM protocol handling updated to Core V4 spec.
- BLE: Added logic to check SN agains previous packet's SN.
-------------------------------------------------------------------------------
Version 2.16.4
October 10. 2012
-------------------------------------------------------------------------------
- Fixed problem with crash when start sniffer for the generic protocol
  with TrxEB+CC112x as capturing device.
- Calculation of RSSI for CC112x updated.  
-------------------------------------------------------------------------------
Version 2.16.3
September 10. 2012
-------------------------------------------------------------------------------
- Fixed problem with warning when selecting protocol version "PRO/2007".
- Calculation of LQI updated.
- Fixed Problem with warning about "invalid payload format" and 
  "invalid LQI/RSSI format".
-------------------------------------------------------------------------------
Version 2.16.2
April 24. 2012
-------------------------------------------------------------------------------
- Fixed problem problem with path to Packet Sniffer FW.
-------------------------------------------------------------------------------
Version 2.16.1
April 10. 2012
-------------------------------------------------------------------------------
- Fixed problem with USB Dongles as capturing device.
-------------------------------------------------------------------------------
Version 2.16.0
March 12. 2012
-------------------------------------------------------------------------------
- Support for TrxEB + CC1125EM.
- Fixed problem with enumeration of USB devices on Windows 7.
-------------------------------------------------------------------------------
Version 2.15.2
July 26. 2011
-------------------------------------------------------------------------------
- Fixed problem with attempt to load FW on USB dongles.
-------------------------------------------------------------------------------
Version 2.15.1
July 6. 2011
-------------------------------------------------------------------------------
- Support for TrxEB + CC1101, CC110L or CC113L
- Support for FW update of TrxEB
- Fixed problem with register settings for CC112x
- Fixed problem with progress indication while FW update running.
- Enhanced register addressing scheme.
-------------------------------------------------------------------------------
Version 2.15.0
Juni 8. 2011
-------------------------------------------------------------------------------
- Support for TrxEB + CC112xEM
- New structure for config files.
- Bug fix for RF4CE
-------------------------------------------------------------------------------
Version 2.14.0
March 14. 2011
-------------------------------------------------------------------------------
- The default folder location for the installation has been changed. 
  All SmartRF tools will be installed under "SmartRF Tools".
- This version includes the signed USB drivers. This means it can be used on 
  64 bit Windows Operating systems. 
- New feature for sharing data stream on UDP port. 
  Includes Perl and Phyton script examples.
- New feature to launch the packet sniffer by clicking on *.prs file.
  Includes readme file with description of how to associate the .prs file
  with the packet sniffer.  
- More information in the status bar.
- Handling of invalid Device ID improved.
-------------------------------------------------------------------------------
Version 2.13.3
October 22. 2010
-------------------------------------------------------------------------------
- The FW for CC2540 USB Dongle has been updated to solve problem with the
  initiator address handling.
-------------------------------------------------------------------------------
Version 2.13.2
October 18. 2010
-------------------------------------------------------------------------------
- The Address book for Bluetooth Low Energy has been changed.
- The byte order of initiator address changed when configuring the radio.
-------------------------------------------------------------------------------
Version 2.13.1
October 13. 2010
-------------------------------------------------------------------------------
- The clock multiplier initiated on startup.
- Problem to turn off filter function if wrong syntax given.
- Improved stability of the FW on the capturing devices.
-------------------------------------------------------------------------------
Version 2.13.0
July 22. 2010
-------------------------------------------------------------------------------
- Added support for the BLE Protocol with possible capturing devices:
  SmartRF05EB + CC2540EM and CC2540 USB Dongle.
- Setup tab cleaned up and changed to "Capturing Device".
- The "Radio settings" tab replaced with "Radio Configuration".
- All configurations related to the radio moved to the "Radio Configuration" tab.
- The RAM buffer size and Clock multiplier inputs moved to the settings menu.
- The mouse wheel can be used in the packet list window.
- Bug fix for parsing of the RF4CE protocol. The message integrity code was
  not handled and therefore the encrypted payload data was wrong.  
-------------------------------------------------------------------------------
Version 2.12.3
January 28. 2010
-------------------------------------------------------------------------------
- Automatic check of FW on CC Debugger.
- FW for CC243x updated to solve problem with time stamp.
-------------------------------------------------------------------------------
Version 2.12.2
November 19. 2009
-------------------------------------------------------------------------------
- Solved problem to use SmartRF04EB + CC2431EM as capturing device
-------------------------------------------------------------------------------
Version 2.12.1
November 09. 2009
-------------------------------------------------------------------------------
- Solved problem with parsing of SimpliciTI data when CC2531 USB Dongle
  used as capturing device.
- Solved problem to read data back from file when chip type is not known.
-------------------------------------------------------------------------------
Version 2.12.0
July 28. 2009
-------------------------------------------------------------------------------
- Added support for SmartRFxx10 kit. 
  Selection of USART interface in Setup panel in order to select correct FW.
- Added support for CC2511 and CC1111 Dongles.  
- Only the protocol must be selected to start the sniffer. The startup is no
  longer dependent on the chip type.
- The firmware files (.hex) has been put together in one common folder.
  Previously the hex file was given ones for each plugin.     
- Solved problem to save packets > 127 bytes to file.
- Solved problem with filter conditions where the values was given with spaces.
-------------------------------------------------------------------------------
Version 2.11.2
Mai 08. 2009
-------------------------------------------------------------------------------
- The startup menu has been changed to include entry for CC2531 USB Dongle.
-------------------------------------------------------------------------------
Version 2.11.1
April 17. 2009
-------------------------------------------------------------------------------
- Support for version 1.1.0 of SimpliciTI
- Support for ZigBee 2007 Pro
- Support for RF4CE added.
- Support for SmartRF05EB + CC2530EM
- Support for CC2531 USB Dongle
- SimpliciTI: The layout of the Management::Address field has been changed to 
              a byte array.
-------------------------------------------------------------------------------
Version 2.10.1
August 18. 2008
-------------------------------------------------------------------------------
- Bug fix of the timestamp for SimpliciTI.
- Bug fix for "Link" and "Join" packets SimpliciTI 1.0.6.
- layout of Source/Destination address for SimpliciTI changed. 

Trouble shooting:
- Problem with downloading new Firmware on SmartRF05EB.
  If the SmartRF05EB board contains an older version (< 0004) of the firmware, 
  the packet sniffer will give a warning and ask for an automatic update. 
  If the user clicks "OK" the update of the FW should normally be executed. 
  If the tool gives an error message saying that the EM module must be removed, 
  the new FW must be programmed with our flash programmer. 
  The flash programmer can be downloaded from our web pages. 
  E.g. from the CC2520DK page: http://focus.ti.com/docs/toolsw/folders/print/cc2520dk.html
  See “Support Software”. 
  
-------------------------------------------------------------------------------
Version 2.10.0
July 23. 2008
-------------------------------------------------------------------------------

- Support for version 1.0.6 of SimpliciTI.
  The SimpliciTI sniffer can now be executed with the following HW:
  SmartRF05EB + CC2520EM. 



-------------------------------------------------------------------------------
Version 2.9.0
July 18. 2008
-------------------------------------------------------------------------------

- Support for packet sniffer on SmartRF05EB + CC2520EM
- Support for general plugin for CC1110, CC2510 and CC2520.
  This means that the payload will be shown as one field with a consecutive sequence of bytes.
  The sniffer can be used to capture packets from any protocol as long as the correct radio settings are used.
- A new startup window to launch the packet sniffer.       
  