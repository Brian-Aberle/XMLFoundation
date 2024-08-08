# XMLFoundation
 XMLFoundation is the fastest XML Parser available.  It is also a framework foundation for an application that uses XML to enable Object-XML mappings and Application Layer binding to XML Data objects.

 This is the article found at : 
 
 https://www.codeproject.com/Articles/37850/XMLFoundation
 

A Big Finish and a Fresh Start
The XMLFoundation has had a 64 bit build for a long time however many 32 bit limitations still applied - until now in August 2024.  Updates and Inserts via XML into StringLists had never been fully supported until now.  .NET was largely unsupported until now requiring interface changes anywhere that a null const char * string was allowed to signify "argument not supplied" within XMLFoundation interfaces because the Marshalling of String type cannot send NULL strings, only empty strings. Winsock2 replaced Winsock in this release.  This release made source code changes library wide - too many changes to list individually.  A file compare utility will be the best way to see all the source code changes.  This marks the beginning of a new era for the XMLFoundation library which has never been assigned version numbers - only release dates.  Welcome to Release Candidate 1 of v1.0. The .NET interfaces are still solidifying.  

I hope to enlist feedback regarding the .NET Interop interfaces.  In the past, most user supplied code enhancements originated through the discussion thread following this article on CodeProject.com.  This is the first release that is also in a supported at a GitHub project which will allow better source code management for user contributions and source code forks.   https://github.com/Brian-Aberle/XMLFoundation

OpenSSL has been removed from the XMLFoundation.  Cipher algorithms used by the XMLFoundation are included within the XMLFoundation to eliminate dependencies and simplify the build. SHA512 now compliments SHA256(currently 2 implementations of SHA256 that will eventually consolidate), and Rijndael compliments TwoFish as symmetric block ciphers built into the class library so that no additional DLLs or link libraries are necessary. Likewise, HMAC for Proxy Authentication is also of the algorithms within the XMLFoundation library.  Simplification of a complex build is what had first inspired putting all of zlib into a single source file [GZip.cpp] - which is now updated to the most recent zLib source distro.  NOTE that none of the new examples use traditional library linking - instead the entire library is #included and compiled inline to the host application or host library making use of #include <XMLFoundation.cpp> from within a .cpp file - an unusual practice in C++ development style.  This ensures that the library builds with DEBUG symbols for any DEBUG application build and uses the same CRT libraries as the application.  The unusual style is not forced upon the application - its merely an option in addition to the more typical link to a static XMLFoundation.lib or XMLFoundation.a 

.NET MAUI-Blazor Integration
The XMLFoundation contains some new example programs in the V1.0 RC1 August 2024 release.  The XMLFoundation Library also targets 32 and 64 bit ARM processors v7 and v8 respectively in a new MS Dev Studio project titled XMLFoundationLibAndroid within the project workspace that uses the CLang compiler.  The Microsoft Studio project file and the source tree have been heavily reworked and rearranged.  User feedback is welcome.  I was never able to properly hook the debugger in the C++ layer DLL which is loaded by the C# layer Interop DLL in the new MAUI-Blazor example application.  From the C++ DLL, I temporarily used an appended to log file with the hard coded the output of certain variables and app logic routes using GString::AppendToFile() (as i have done in various other times, situations, and platforms where the debugger is not available).  Additionally, it was easy to develop portions of the code in a minimal .EXE where the debugger is able to help tremendously prior to moving the code into the C++ DLL within the .NET MAUI Blazor example .  I saved that simple app, which is a console version of the logic found in  .NET MAUI Blazor example - That project is titled "Simple" in the workspace currently containing 32 projects.  A few projects still remain outside the larger Microsoft Visual Studio workspace and outdated projects were removed.

Ubuntu
The XMLFoundation Library and every C++ example program works on linux and has been tested in this release using the most recent versions of gcc and Ubuntu.  Running the example program ExIndexObjects is impressive on Linux.  That application allows you to compare execution times for insert, search-find, search-nofind, full-iterate operations on multiple indexed data structures containing objects created from XML.  The execution times in a Linux VM are faster than on the Windows host machine although the host had more reserved CPU.  

Android Studio 
"Design Pattern Native"
There is a new and in-progress project for Android Studio using Kotlin in the Gradle build system under XMLFoundation/Examples/AndroidStudio titled "MyApp".  This project currently builds the entire XMLFoundation C++ library source code - including ServerCore.cpp into a per-application native library that the Java application layer can use.  The "MyApp" project is structured in a way that compiles the XMLFoundation code base into an application specific library which is built along with the Java user interface code that will use it (see native-lib.cpp in the MyApp example).  This is the exact same application design pattern and the actual source code used by the .NET MAUI Blazor example which implements the same idea for interop between C# and C++ that the Android Studio app does between Java and C++.  BOTH the Android Studio example and the .NET MAUI Blazor share the same C++ example application implementation layer found in the source code files [XMLFoundationApp.cpp/h] and [ExampleObjectModeling.cpp/h].  The interface from C++ to Java found in [native-lib.cpp] in the Android Studio "MyApp" example is what [DLLExports.cpp/h] is to the .NET MAUI Blazor app.  This example app is still a work in progress.  I decided to release it prior to completion because the remaining work is mostly basic user interface Java code to be written.  This example builds for ARM processors, v7 and v8 - 32 and 64 bit respectively the build fails for x86 and x86_64 I would remove those targets until they are working - but i dont know how.  If an ARMv7 virtual android device is running in the AVD Android Emulator then Android studio will run the application there.

"Design Pattern Java"
The XMLFoundation source distro has contained an example for Android Studio since 2013.  It was developed using Gradle version 2, and supported Eclipse as well as Android Studio.  That project will not build using the current version of Gradle.  The emphasis of that example was using ServerCore for tunneling and routing of any TCP application with an example for VNC which has been removed from the workspace because VNC as the example TCP app no longer worked on modern versions of Android.  The original GApp example also showed how to use [JavaXMLFoundation.cpp] with [ObjectModel.java] which is derived from [XMLObject.java].  Both java source files are located in [XMLFoundation/Examples/Android/XMLFoundationProject/GApp/src/main/java/a777/root/GApp ].  This design pattern for handling XML gives the software developer a "Pure Java" development solution based on the JavaXMLFoundation.cpp which requires no C++ code to be written.  Although the unsupported Gradle build file prevents the example from building, all of the old source code which is still relevant, and still compiles, still remains in this release for reference.  A new example project file for the "Design Pattern Pure Java" is on the TODO list.

Important Update to Licensing Agreement
If you incorporate this code into your own, a simple comment anywhere within your own source code giving credit to XMLFoundation fulfills the copywrite agreement.  You have been advised of the important update to the licensing agreement.  This is free and unencumbered software released into the public domain.

Important Stability Fix in September 2023
There was a fix for handling the situation of CDATA in the input XML mapped to objects not derived from XMLObject. The unusual, or unexpected, XML can cause application failure, therefore, it is advised to rebuild with the latest source code if the possibility exists that CDATA could appear in your input XML where you do not have it handled. The change being in XMLObjectFactory.cpp. additionally empty CDATA is now handled properly with a change to xmlLex.CPP. Also there is a small change to SHA256.CPP for g++ -O0 compiler options.

Don't expect more fixes, as this was the first in years after the many people have been using this code base. Support for the latest development tools will continue. Terrance McKenna said that there is a link between human technology advancement and consciousness expanding neuromedicine. After decades of research, I have a thesis that details how this can be and how neurodegeneration and cancer proliferation are involved: (PDF) Harmine and the Beta-Carboline nutrient complex: Neurological and Psychological Effects of Peganum Harmala: Neurogenesis, Entheogen, Alzheimer’s, Cancer and Antidepressant (researchgate.net)

VS2017 Best Integration since VC6
This is what was new in September 2017.

The XMLFoundation distribution now includes a native VS2017 workspace.

The XMLFoundation makefiles were ported to VC6 in 1999, many source files were then developed and debugged in VC6 and ported to Unix. (People Still use VC6 today: See Best C++ Article of the month June 2017). The VS2012 workspace was auto generated by importing the VC6 makefile, that VS2012 makefile was auto-imported into VS2013 and VS2013 into the VS2015 workspace. Over the years, a few issues propagated through the Visual Studio makefiles for XMLFoundation such as the debugger not launching the executable because it could not be found - although it was generated. The new VS2017 workspace has reorganized the build for each target of - Debug 32, Release 32, Debug 64, Release 64, and 64 bit Unicode to work with the new directory structure for managing the openSSL libraries and corresponding header files in the XMLFoundation source code distro. It allows for XMLFoundation to be added into a build structure with a preexisting openSSL source structure or into an application with nonexisting openSSL source. VS2017 has the best XMLFoundation build integration since VC6 because it has been manually configured for every target.

Amid the move to the most recent Visual Studio, was a move to the most recent Microsoft branch of OpenSSL from here: https://github.com/Microsoft/openssl/. While building OpenSSL (properly with NASM) - for 32 bit targets I found that OpenSSL builds fine from the command line using VS2017 or VC6 if the VC6 (Unofficial)"Service Pack 7" files are copied from [XMLFoundation\Libraries\VC6Winsock2] into the VC6 program folder [Program Files (x86)\Microsoft Visual Studio\VC98\Include] to update VC6 command line builds with Winsock header files that contain IPv6 declarations. Also copy WS2_32.Lib and WSock32.Lib into the [VC98\Libs] to update the object file code that gets linked into your binary executable code or app. Interestingly, when building the most recent openssl code with the most recent 64 and 32 bit compilers, there are a few unresolved externals which are items missing from the C Runtime libraries, when using new compilers - __iob - _fprintf and __vsnprintf. The unresolved items are resolved in ServerCore.cpp - preprocessor directed on a per compiler basis so that XMLFoundation compiles on many platforms - past and present.

Paving Trails on the Information Super-Highway
This is where we were at in July of 2016: Android Studio 2.2 preview 6 does not allow you to change the NDK directory, previous versions did. In the most recent NDK(r12b), I could not link the most recent stable openssl(1.0.1i). The issue was unresolved at Stackoverflow here. I also found this open issue.

So – Since I cannot rollback to an NDK with the required symbols in the platform, I was forced to modify the openssl source code to remove the dependency on the platform support. What I did was the same fix used to cut out shell support on Windows CE builds by using preprocessor directives in [ui_openssl.c] This is the beauty of open source, so I built the [modified]most recent stable openssl (1.0.1i) for Android platforms: armeabi, armeabi-v7a, arm64-v8a, x86, and x86_64.

Unbeknownst to many, Android (v6+) no longer uses OpenSSL. https://github.com/android/platform_external_openssl - Since GoogleAPIs are not using OpenSSL, it's likely not an issue that their development teams are facing. The GoogleAPIs are not open source, so if you build your application on them, it is reasonable to expect that your processor, bandwidth, and privacy will be taxed, therefore giving rise to https://microg.org/.

Android Studio - Gradle Ahead
When I last published the XMLFoundation Android examples, I was using Android Studio 1.31 (Build date: Aug 3, 2015). At Google I/O May 2016 Android Studio 2.2 Preview was launched, a large update in many respects. “We are working on a new build system to replace both the build system inside ADT and Ant.” According to http://tools.android.com/tech-docs/new-build-system

XMLFoundation is embracing the new Gradle build system for Android builds. Setting up a proper Gradle build system can be confusing because most Android project build examples use older Gradle plugins. The new Android Studio (using version 2.2 Preview 6 built on July 18, 2016) example uses the new, more organized, build structure during a time where a few examples are the best documentation. The NDK examples now include the new examples “MoreTeapots” and “CMake” which contain helpful direction just like the new Android example titled “XMLFoundationProject” does which targets ['armeabi', 'armeabi-v7a', 'arm64-v8a', 'x86', 'x86_64'], and links the openssl libs crypto and ssl accordingly. Inside the “XMLFoundationProject” are several modules, one titled “XMLFLibrary” builds the foundation into a shared dynamic library, and GApp uses all static linking in the "New School" build style.

The example builds in Windows and Linux. See XMLFoundation/Examples/Android/Using Android Studio 2.2.txt.

Heaping on Speed in 2016
Each thread has its own heap. When a thread is destroyed - so is its heap. This allows us to disable all memory cleanup in many cases when we plan to destroy a thread and use a new thread for the next transaction. You might not think that such a simple idea will cause the XML parsing performance to double (aka happen twice as fast) or even triple. The memory manager in the operating system must block in the delete() implementation, blocking is slow. We can't stop calling delete() just because it's slowing things up - or can we? See the new example titled Threading. This design pattern cuts "initialization time" out of "transaction time", as well as cutting out the big time pit that (unnecessary) destruction is. Memory managers optimize new over delete, so we know why the black hole time pit exists. It takes longer to put memory back than to obtain it, the world would really be messed up if it was the other way around. We can take advantage of the situation for a massive performance enhancement. In 2009, I wrote the section Faster than Fast. In 2011, I expanded on the concept with Custom Memory Management. Using heaps efficiently expands on the same concept once again so this is faster than the old 2015 version.



3.33 Times
This paragraph was in the September 2015 update. Moores Law, which has held mostly true for a few decades, says that the rate of processing speed doubles every two years. Times are changing, infact Gallium Nitride will replace Silicon. Saying Silicon Valley will be like saying 8 Track Valley. You’ve gotta move with the times. I bought a development machine with a higher end processor about a year ago, recently I swapped the main disk with an SSD to see it boot 3 times faster and to see my VM’s start and stop even faster than that. Three times is a lot. It’s like being able to use your brain after opening up the bottle neck. When using the OID object IDs, XMLFoundation does parse XML 3 times faster than other XML Parsers due to its unusual design that never copies memory to an intermediate tree between the linear input buffer and the application layer which also effectively uses half as much memory while producing the 3X speed difference – it’s no more revolutionary than an SSD.

Platform Expansion
This is what was new in August 2015: A while back, I made a future reaching decision to include the big fat openssl libraries into the XMLFoundation. They make the download huge and most of the examples do not use the openssl libraries, most people who use the XMLFoundation do not want openssl especially a binary for Windows Phone. Despite all that, those libraries need to be there to see the NTLM authentication work on all platforms that XMLFoundation supports. Now the XMLFoundation is reaping the rewards from integrating openssl. Consider the class CSmtp, added into the XMLFoundation during August 2015. That project grew up and matured here on CodeProject. The source code download in that project only contains one platform build of openssl binaries. I integrated CSmtp into the XMLFoundation and added support for all Microsoft compilers32 and 64 bit targets with the correct openssl libs ready to link as well as a Linux makefile. CSmtp is portable and it was well done, kudos to the builders. The CSmtp source download comes with openssl-0.9.8l, In the XMLFoundation, CSmtp uses newer versions of openssl. The big fat binaries are being shared by the NTLM code and the TLS code now - on many platforms.



GProcess
Just because some new class or functionality shows up in the XMLFoundation in August 2015 does not mean it is something new or experimental. I included a portable routine that returns a list of processes. It works for Windows CE, Windows 98, Windows Phone, Win32 and Win64, Android, IOS, and Linux. It obtains all the process information available if running as Administrator or it obtains a lesser set of process information available when running as non-administrator in Windows. It works everywhere, with all compilers, it is well debugged and it is a worthy contribution to the XMLFoundation. The new code is in GProcess.cpp and the new Example program SMTPandTLS shows how to use it.

VS2015
I updated the VS2012 and VS2013 workspaces. I recently verified every XMLFoundation example program in 64 bit, 32 bit, Debug, and Release under VS2013 . A while back, I decided to stop "migrating" the workspace into new Microsoft compilers. Instead, I create a new workspace for each compiler, this works in more environments. In the case of VS2015, keeping the other workspaces reveals major differences in the C-Runtime that the compilers use. When I was a young cocky C++ programmer, several times I thought I found bugs in Microsoft code only to later discover that I was somehow misusing some under documented API call. Now I am well "seasoned" (even smoked and marinated - been through fire and flood). I am slower to hurl bug accusations at Microsoft, however I did find a few bugs in the C-Runtime libraries that ship with VS2015. Hopefully, this problem will be corrected in the next Service Pack since that compiler is still supported. The bug that I found causes the link to fail on all example programs that require openssl in VS2015. Look at the first two pages of source code in CSmtp.cpp to see the details on this bug. The VS2012 and VS2013 compilers produce working binaries of the same example. This is why I still use VC6.

Networked
The new sample application titled SMTPandTLS gathers up this information and emails it to you like this:

Shrink ▲   
----------------------------Wan IP----------------------------------------

73.3.213.175

----------------------Network Interfaces----------------------------------

Host:MY_COMPUTER_NAME

Type[5]:10.0.0.6

Type[1]:127.0.0.1

Type[1]:192.168.3.1

Type[1]:192.168.121.1

----------------------Network Connections----------------------------------

TO=23.195.144.35 port:47873    FROM=10.0.0.6 port: 39872       C:\VMware Workstation\vmware.exe

TO=23.99.205.208 port:47873    FROM=10.0.0.6 port: 59601       C:\Internet Explorer\IEXPLORE.EXE

TO=65.55.246.20 port:47873      FROM=10.0.0.6 port: 57298       C:\Internet Explorer\IEXPLORE.EXE

TO=204.79.197.210 port:47873  FROM=10.0.0.6 port: 57810       C:\Internet Explorer\IEXPLORE.EXE



------------------------Processes-------------------------------------------

pid:3288   iusb3mon    [Intel(R) USB 3.0 Monitor] C:\Program Files (x86)\Intel\Intel USB3 
pid:460   IAStorIcon     [C:\Program Files\Intel Rapid Storage Technology\IAStorIcon.exe]

pid:980   iexplore#1     [C:\Program Files (x86)\Internet Explorer\IEXPLORE.EXE]

pid:2596   firefox     [[XMLFoundation - CodeProject]

pid:32   http://www.codeproject.com/Article] C:\Program Files (x86)\Mozilla Firefox\firefox.exe  

pid:6716   SMTPandTLS     [C:\XMLFoundation\Examples\C++\SMTPandTLS\Debug\SMTPandTLS.exe] 
That's what's new for August 2015. Let me hear from you in the comments below, Gimme a high 5 or something. I work hard to put this code together clean and clearly labeled, again you will find the new code strategically commented with helpful insights.

This Threaded Thesis
I have written multiple kinds of TCP servers and a few kinds of TCP proxies (conversational like VNC and Telnet and transactional like HTTP) so it helps that I have done this kind of thing before. ServerCore, at the very core of it is not a server at all - it's a "TCP Socket to Thread of Execution manager" but that might confuse people and I work hard to keep this stuff simple. ServerCore.cpp has served on Solaris and AIX and on EVERY mobile platform.

I was once working next door to a company that had 50 million image files on a disk array that their own custom software corrupted the NTFS headers on so the folder of files appeared empty in the Windows Graphical shell explorer and at the command prompt. The file names were completely predictable so the files could be copied by a simple loop that knows the naming scheme and issue a copy like (c:>copy c:\lost data\File00000000000000000000a.jpg d:\found data\File00000000000000000000a.jpg) but the next crisis was that it was going to take 10 days to run the file copy. ServerCore.cpp was used to execute the file copy on <font face="Courier New"> clientThread()</font>. I quick made a GUI for this (a one time use - throw-away GUI) that allowed me to adjust the number of concurrent copy threads and see the change in throughput. I found that with their hardware configuration the best performance was around 7 concurrent threads to maximize the I/O throughput, I could easily measure the difference between 5 and 50,000 threads on their high end hardware which allowed me to measure thread context switching overhead and stress test and optimize the ServerCore threading model in my spare time. The copy was finished in about 50 hours. Is it right to call the copy program a server? I would say no, but I avoid terminology conversations. The threaded copy program was based on ServerCore's threading model which was first designed and used in Unix and NT.

Maximizing the I/O throughput of TCP connections is quite a bit different because generally TCP is an order of magnitude slower than moving data over a disk controller. This means that most of the threads are sleeping most of the time on a TCP server. I applied an experienced approach toward implementing an HTTP Proxy in ServerCore.cpp. It's a complete implementation that runs on all mobile platforms(Android, iOS, Win Phone) and many server platforms. In the past, an HTTP Proxy was generally only the function of a high end server. Nowadays, every phone and every tablet can act as an HTTP Proxy to extend Internet connectivity to devices near them.

We all know that a picture is worth a thousand words, and I will refrain from many more words on this topic because the source code is very well commented so there are more words down in the code for folks who want more words. This is the basic logic flow. It takes two threads for an active proxy: listenThread() is in a tight loop that is either sleeping or accept()ing a socket that will be handled by a clientThread() in the pool, clientThread() gets a ProxyHelperThread() from another pool. Inside ServerCore.cpp is also an HTTP server (which has its own instance of a listenThread() on another port) The HTTP Proxy and the HTTP Server are both implemented in clientThread() - but only the HTTP Proxy uses a ProxyHelperThread(). clientThread handles A and B, the ProxyHelperThread handles C and D in a loop that is mostly sleeping until data arrives.


The source contains a new project titled HTTP Proxy Server that compiles into a Windows Service. The Windows service itself is a reference implementation, complete with command line arguments to install, uninstall, start, and stop the service and set it to automatic or manual start. The full set of options are documented in the source code. The "HTTP Transactional" logging could be helpful to web developers, when it's turned on, it creates files with names based on the HTTP Transaction.

nside the .OUT file, we see the data sent out for that HTTP transaction, being able to sort the individual transactions by name, server, and size is handier than reading a Wireshark log. Other than for reverse engineering someone else's website (or debugging your own), this wouldn't be handy for much.

GET /Articles/37850/XMLFoundation HTTP/1.1
Accept: text/html, application/xhtml+xml, * / *
Accept-Language: en-US
User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; Trident/7.0; rv:11.0) like Gecko
Accept-Encoding: gzip, deflate
Proxy-Connection: Keep-Alive
DNT: 1
Host: www.codeproject.com
Cookie: __utma=40492976.65300.139906.1416767222.1416957829.696 
and in the .IN file, you would see a response that starts like this:

HTTP/1.1 200 OK
Cache-Control: private
Transfer-Encoding: chunked
Content-Type: text/html; charset=utf-8
Content-Encoding: gzip
Vary: Accept-Encoding
Set-Cookie: SessionGUID=7ed7a38c-d059-4f14-86ea-0fef4c82d576; path=/
Set-Cookie: mguid=1fc5162-e564-458c-8180-b42cec55ce5; 
            domain=.codeproject.com; Date: Tue, 25 Nov 2014 23:24:47 GMT

1072e  (followed by the data) 
As I said above, ServerCore.cpp also includes an HTTP server that I have running on port 80. I also have my own protocol implemented in port 10777. I have the HTTP Proxy configured on port 8080. Down in the gut of ServerCore is a little function showActiveThreads() that returns real-time thread state for every clientThread(). It returns the data in a comma separated list that you could view many ways in your own application, this is a screenshot of the information displayed in a GUI that allows you to sort individual connections by port, IP, connection time, or action. When looking at many connections, the sort on Action helps to group and identify connections.


2014 The Year in Review
November 26, 2014, this was what I had to say: On January 11 - 2014, I published a performance breakthrough that dramatically improved the performance of what was already the fastest XML Parser on earth. The new breakthrough of memory management on top the breakthrough of the "oid" put XMLFoundation in a league of its own when doing XML performance tests. In search of some official feedback, I "Requested For Comments" at IETF (Internet Engineering Task Force) in [apps-discuss]. I explained the "oid" enhancements made to XML that are required to achieve a three fold performance gain over any SAX or DOM XML Parser. I did get a little feedback, but there was an argument from senior members that the issue before them was not in their jurisdiction so I was recommended to bring the issue to [xml-dev]. So I did. Talk of XML 2.0 is like talk of changing the Bible to some people. The discussion took over the list servers until some political bug at Oasis brought the list servers offline for almost 5 days. Since [apps-discuss] had been on the topic of SMTP at the time, I started a thread [Help! SMTP has stopped Working] and asked for advice. Here are a few parts of that public conversation about how XMLFoundation uses the "oid" as the 1st attribute to key object instances at [xml-dev]. We can can rightly call these folks experts.

http://www.linkedin.com/pub/hans-juergen-rennau/11/6a9/5b2 Hans-Juergen Rennau "The OID pattern to which Brian Aberle introduced us seems to me very interesting, as it throws light on a peculiarity - perhaps weakness - of the XML information model: the XML model is unrelated to the concept of a resource as it underlies the web..... I think the OID may be regarded as a special kind of resource locator, URL, that is."

https://www.linkedin.com/in/peterhunsberger Peter Hunsberger: "I now understand that you are attempting to optimize the management of individual instance data, across the web"

http://www.xml.com/pub/au/206 Len Bullard: Creating objects with markup is a perma-thread from the early days. Tim Bray called such ideas "premature optimization"

Brian Aberle wrote: > [...] If the world was to increment the XML spec...

http://www.w3.org/People/Quin/ Liam R E Quin: "Unlikely, I'm afraid..."

Liam R E Quin: "Although there's certainly interest in making XML faster, it has to work e.g even if attributes are re-ordered or rewritten."

www.linkedin.com/pub/amelia-a-lewis/23/11a/a20 Amelia A Lewis: You do realize that this means "this is not XML"?

http://www.linkedin.com/in/garethoakes Gareth Oakes: "I can think of a few edge cases where an XML caching mechanism may be vaguely useful but I am intrigued about your use case for this."

http://en.wikipedia.org/wiki/Michael_Howard_Kay Michael Kay had The Best Question Asked, it was "What scope is the OID unique within?"

It was the best question because it is an exact question with an exact answer that drew an alternative implementation using XML's Process Instructions which I had never considered but once brought to light during this conversation recognized was possible, the drawbacks being more markup overhead, and the added liability that a processing instruction must always be hand copied along with the XML to which it applies if the XML is ever hand coped increasing the chances of human error due to lack of markup knowledge, whilst the OID as 1st attribute keeps the association with the XML/Object to which it applies. Still the argument is valid and such an implementation can achieve the bulk of the performance gains.

http://www.noaatech2002.noaa.gov/abstract_21.html Thomas Passin - "As I have been following this thread, it's seemed to me that this is exactly a case for a Processing Instruction."

http://www.linkedin.com/pub/hans-juergen-rennau/11/6a9/5b2 Hans-Juergen Rennau

Gentlemen,

I am not sure if such historical facts and details [about why XML does not address attribute ordering] are really important in the present context. At any rate, what interests me is the relationship between Brian's initiative and current XML. And what strikes me is the following. The XML model defines the information content of a given document; a document is the content which it is, and any glimpse beyond the document is out of scope. In particular, there is no room for distinguishing between a resource and its representation, - resource and representation are always one. But Brian's approach, so it seems to me, would build into the information content of a document a statement establishing a relationship with a seperate instance of information content (the data referenced by the oid), assigning to one (the data containing the oid) the role of being an update of the other - assigning to both the roles of subsequent states of the resource which assumes those states, but is not identical to them. And this is certainly an interesting idea.

Hans-Juergen Rennau

Why XML Fails to Address Attribute Order?
Within the discussion about the OID was the lingering unanswered question about why the XML specification does not address attribute order.

http://www.xml.com/pub/au/82 Simon St. Laurent: "I've always thought that discarding attribute order was one of the unforced errors of the XML spec "

www.linkedin.com/pub/michael-sokolov/4/a4a/235 Michael Sokolov: [ Answers the outstanding question with speculation (that I speculate must be right) ]: " Hashmaps do not preserve insertion order, so it may be that this was a rationale for discarding the order of attributes. I don't know. If so it wasn't a particularly good rationale."




You Can Quote Me On That
This was the May 2014 Update.

Software projects can fail for many reasons. In mission critical software projects, there is a common objective to avoid design-resource dependencies. If one critical resource is run over by a bus, the whole project can become "bus terminated". In a lesser "mission critical" context, when another company hires away your undervalued programmer - then management finds out that the new guy can't just pick up where the undervalued guy left off. Some smaller thinking programmers even convolute code, or leave out a few necessary comments just for job security. I am normally here to support anybody using XMLFoundation in some way that uncovers an error or missing feature - however many external events could "bus terminate" this "live" support. XMLFoundation is designed and organized to prevent failure and stand the test of time. There are no outstanding bugs. There are no feature requests awaiting implementation. There are no disputes to the claim that this is the worlds fastest XML Parser. XMLFoundation is being used more than ever before and at the same time, the need for me to answer questions has decreased due to the heavily commentated and exemplified code. At this point, termination of myself will not be the termination of a project based on XMLFoundation. The next economic gut bubble that vents it self wont blow XMLFoundation away.

I remember when XML 1.0 was disruptive. The sudden appearance of a global standard immediately forced projects worldwide to change course, not all could. Projects that could not react to the disruptive technology (or the opportunity) are now built on less standard protocols that now excludes them from some new use case scenarios or forces them to add layers of inefficiency and failure points in retrospect design. It's no wonder why software projects are so well known for failure. Many things can go wrong, and times change fast. If you search the internet for the term "Disruptive Technology" along with "Capitol" or "Venture", you will see that "Disruptive Technology" is sought after and valued even though it disrupts culture and/or other businesses. XMLFoundation is Disruptive Technology. It's like what vehicle radar detectors once were to police radar guns. Is that Disruptive Technology or Defensive Driving? Technology defends us from technology at every level. The performance numbers XMLFoundation produces does disrupt some technology business plans. In most technology plans, XMLFoundation merely adds new options.

When referencing this "Article" at [xml-dev], I called it bloggish. I remember reading Stroustrups First edition of "The C++ Programming Language", which had interesting quotes in it, here are a few re-quotes from the Fourth edition:

Don't interrupt me while I'm interrupting.
-Winston S. Churchill
Premature optimization is the root of all evil.
-Donald Knuth
On the other hand, we cannot ignore efficiency.
-Jon Bentley
The purpose of computing is insight, not numbers.
-R. W. Hamming
... but for the student, numbers are often the best road to insight.
-A. Ralston

In his book, Stroustrup injects some "bloggish" quotes like this:

"... there is nothing more difficult to carry out, nor more doubtful of success, nor more dangerous to handle, than to initiate a new order of things. For the reformer makes enemies of all those who profit by the old order, and only lukewarm defenders in all those who would profit by the new order..."

-Niccol `o Machiavelli

Other Machiavelli Quotes that didn't fit in "The C++ Programming Language"

When I saw that Addison-Wesley allowed Mr. Stroustrup to publish "Programming is like sex..." in "The C++ Programming Language", then I knew that it's OK to have a little bit of fun with this stuff as we teach. So check out the new section in this documentation - The Sexy GString.

There are some heavier areas of exemplification and documentation that still need to be done, specifically in the areas of Distributed Object Designs, and XML-DBMS integration and the Java integration, these areas and topics are as foundational in XMLFoundation as the GProfile, which just now got its first real bit of documentation. If anyone would like to rent my hourly help, we will help each other contact me: XMLBoss at live dot com.

Programming is like sex: It may give some concrete results, but that is not why we do it.
-Richard Feynman

What is the XMLFoundation
XMLFoundation is a cross platform application foundation. GString and xmlLex are 1 and yet they can be separated. The G classes are "designed" for each other - not simply "used" together. The GString now supports Compress and Cipher as member methods. Since both GZip and TwoFish are very portable and since the GString is designed for binary data equally as much as for character string data - the combination is a natural fit that deserves to be in the base class of GString. This comes after synchronizing GZip.cpp in the XMLFoundation with the latest source code from http://zlib.net/, an update that includes a fix for a rare compression bug. Zlib implements compression for most applications built after 1996. The authors recommend that all users of Zlib should upgrade and obtain the fix.

The XMLFoundation library contains many things. From utilities to frameworks, (plural to plural). The XMLFoundation implements a use of XML that it is about 3 times faster that the best that can be done with SAX, when parsing XML for the purpose of an update to some dataset that you already have. It is a very common task in the application layer or in a DBMS that uses XML as a native input protocol. In that case, if a key word OID is present as the first attribute we can be triple fast AND use 50% less memory. This is accomplished by eliminating a temporary memory copy made by typical XML parsers as well as eliminating many arguments being pushed and popped from the stack by using xmlLex::getToken(*tok) explained in more detail where performance is discussed.

The fact that the first 3 letters are ALL CAPS of in XMLFoundation understates the C++ FOUNDATIONAL application development tools ( which contain an XML Parser (like the C++ framework QT) ). Building on GString is like building on a "better" ostream. Opinion has no place amid "better" performance data. Every application that was ever built on ostream could benefit from the XMLFoundation. All the utilities are carefully designed to be a portable platform everywhere that a C++ compiler exists (like the popular C++ library Boost (which XMLFoundation compliments nicely) ). XMLFoundation's GThread is still the only pthread port for Windows Phone that I know of. Windows Phone is now the third-largest OS across Europe with 10 percent of the smart phone market -- more than double its share compared with last year. I support it. Behind the Capitol XML is a solid application Foundation.

In the past 30 days, there have been several minor "fixes" in the XMLFoundation as well - so all of this deserved being put together into build 211 that is already serving several production applications. Additionally, new documentation now includes example use of the virtual ToXML() method of XMLObject in the new example program titled BigData. Since XMLFoundation is the fastest XML Parser on earth, it stands as the most viable solution for integration of massive XML data sets - most will be to/from an SQL database - but where BigData meets XML - the XMLFoundation is a solution present. Despite the focus on XMLFoundation support for the equally important mobile platforms (iOS/Android/Windows Phone), XMLFoundation originated from the high performance needs of large data warehousing and is positioned to be the de facto standard for BigData XML just as zlib became the de facto compression. Zlib is officially de facto according to wikipedia - which is often known to be incorrect in certain topics, but I believe it is correct in this case.

Calling All Chiefs for a Pow Wow
This is what was happening on March 31, 2014.

Of recent, I have sought out 'leaders' in the domain of XML to present a proposal for a futuristic XML that has 2 new small keywords that prefix an attribute list in a 100% syntactically backwards approach as to not disrupt any XML 1.0/1.1 software. To assemble the chiefs for a pow wow on the subject requires the attendance from the leaders of many technical worlds. IETF is a moderated forum (like codeproject.com is moderated) and OASIS is unmoderated, once famous for hosting [xml-dev] where interfaces like SAX and DOM were created by unmoderated open debate and consensus.

I have been building and perfecting the examples in preparation for presenting this concept to the industry leaders. The concept is a 'new' idea - or as someone pointed out, it's a variation of a perma-thread from the old days. In this article, I had mentioned something about "Object Databases" almost changing the world once but there was a performance problem - so the world is square.

The discussion got hot at [xml-dev] until OASIS experienced malfunctions that had the list servers down for a total of 4 and a half days. OASIS Tech support had no response to some people and a broken hyperlink for problem ticket 2546 for other people. The discussion lasted for 2 days and brought out excellent debate and advancement in the documentation of the concept. Many "Authorities" on XML commented prior to the server failure that stopped the discussion for almost 5 days. I am trying to communicate this architectural design concept to all who can understand. For those seeking to understand the design concepts, I believe it is best to read the questions asked by "Experts" on the subject of XML and read the 2 day discussion. While discussing 'new' concepts like this, various views and terminology used in other people questions is helpful to read. The discussion at [xml-dev] mostly accomplished documenting the outline for an official 'Internet Draft" based on a summary of this discussion.

 

Please use the forum following this article to document any additional point to cover in a
formal "Internet Draft" document for standards approval - In progress.

Recently, I added a "major" new example program. It's mostly been there all the time but it never had a makefile - see CustomHTTPService. It builds a static web server and implements a simple post handler AND a multi-part-form handler. It's great server platform based on ServerCore.cpp.

Recently, I also added a new example program called ContainOrInherit, explaining the difference between containment design and inheritance design, and the GProfile now supports XML format since that could be useful. See the (large) comment explaining the reasoning for adding XML support to GProfile 14 years after designing it amid all this XML support.

Earlier in March 2014, the bulk of the work was in the files Console.cpp and 5LoavesSvc.cpp - both are excellent frameworks for any service. Those files are not tied tightly to ServerCore.cpp, they use ServerCore.cpp. Those files are a very useful starting point 'design patterns' for many applications. They are intended to be generic for anyone needing to build a windows service. Those two files are not about XML, they are about Foundation.

Custom Memory Management
This is what was new on Jan 11, 2014

This is a point in the XMLFoundation project that I will switch my focus to the porting and development of several applications that were designed and built using older versions of the XMLFoundation library. Recently, my focus had been enhancing the XMLFoundation. I have more than one application that still has yet to be ported to 64 bit. In the future, updates to this main article will indicate a major update versus the daily or frequent updates of the most recent source at the external link. The current XMLFoundation and the examples are very much a result of user feedback. Even the changelog.txt was started in response the suggestion that someone gave me. Good idea. The new ChangeLogTail.txt was my idea. If you come up with a new example program, idea (great or small), or feedback that might help others - don't keep it to yourself - share it with. Share it with myself and the public on the forum at the bottom of this page, or share/discuss it with me directly (Roaring Checkmate At Live dot com). I ask for this feedback despite the fact that recently there has been more "downvoting" than ever before. The old "5 Star" XMLFoundation has new "1 Star" haters.

The very old section in this document titled Faster than Fast, written in 2009, was put together to give some explanation and emphasis to the fact that XMLFoundation is fast. The XMLFoundation is FASTER in 2014 (as much as 3 TIMES FASTER at some operations than any 2013 release). Thank God for numbers, because words can say anything, but numbers divides fact from words. This 2014 speed-leap comes from customized memory management. This is as simple as reducing calls to the generalized global memory manager in the operating system via calls to new() and delete(). The concept is simple, and there are various approaches. The goal is to obtain heap memory for a set of operations that you might define as 1 transaction or complete task.

I have the advantage of having done custom memory management before so I applied an experienced approach toward accomplishing this. In this release, I abundantly commented the source code changes and additions to document exactly how this custom memory management is accomplished. The proof is in the numbers exactly how important of an issue memory management is regardless of your target being on an Android phone or a 64 bit Windows server the same concept applies to memory management.

Additionally, as we kick off 2014 with a massive reduction in the reliance of the operating systems memory manager, we raised the roof for 32 bit processing limits that are reached when new() returns NULL. So, if faster is not what you need, perhaps you will enjoy the higher processing limits that you can now achieve in the same memory limitations that you have always had. FASTER execution and HIGHER limits are both achieved by the same upgrade in this release. I expect that this raised the roof for 64 bit limits as well but I have never seen that roof.

The bulk of the memory management documentation is in the code, I will conclude the introduction to the 2014 version with the basic steps that make it all happen.

In XMLObject.h, the new method:

C++
virtual int GetMemberMapCount() 
returns the number of MemberMaps() that all objects of any specific type will have mapped to them. Knowing this, XMLObject will preallocate a chunk of memory large enough to hold the details of ALL MemberDescriptor()s that contain that information at runtime for the object instance - in the primitive days of 2013, each MemberDescriptor was allocated a global heapspace of its own using new().

The code in XMLObject.cpp now looks like this - to manage the memory blocks:

C++
int nArraySize = GetMemberMapCount(0);
m_pMemberDescriptorArray = malloc(sizeof(MemberDescriptor) * nArraySize); 
GList made did a similiar change. Here is a bit from GListNodeCache that aplies the same concept a bit differently.

C++
void *pBlock = malloc(sizeof(GList::Node) * NODES_PER_ALLOC); 
// Now we access individual nodes like this: The temp variable pVoid is for readability....

C++
void *pVoid = ((char *)pBlock) + ( sizeof(GList::Node)*nBlockIndex);
GList::Node *pNodeInBlock = (GList::Node *)pVoid; 
// this is the same code as above with no temp variable. It is a type cast with pointer arithmetic:

C++
pNodeInBlock = (GList::Node *)((void *)(((char *)pBlock)+(sizeof(GList::Node)*nBlockIndex))); 
Analysis of Algorithms
This is what was new December 21st 2013.

MurmurHash was the latest big advancement in Hashing. It was published in Austin Appleby personal blog. Google hired him and has taken over Murmurhash, and published a variant of it called CityHash. Strangely, many Google publications failed to recognize SpookyHash - published October 31, Ground Hog Day, and other celebrated days, citing only MurmurHash.

Here are two links to help you quickly catch up on hashing algorithms to get your prerequisites up to date. To cut through all the trending Hash Hype, I recommend this brief overview of hashing.:

http://www.homolog.us/blogs/blog/2013/05/06/why-computer-science-professors-dislike-hash-functions/

I wanted to see if CityHash could help me speed up my indexing scheme so I decided to test SpookyHash/CityHash within GHash which is part of XMLFoundation. The test counts CPU cycles and/or microseconds on both Windows and Linux while indexing large datasets for both 32 and 64 bit applications. The source code for this test is included in the example program ExIndexObjects.

The results are very interesting. GHash is so fast that CityHash slows it down. The GHash is a unique algorithm designed to index XMLObjects but it can index anything. In summary, GHash is an Array of B-Trees. It handles hash collisions so efficiently that it eliminates the need for "low collision" hashing algorithms such as CityHash which use far more CPU than a simple Rotating Hash, and further research will determine just how low it can go in the simplification of that step.

CRC-n. is faster than CityHash. For Checksumming, or creating a hash function with a perfect distribution (aka avalanche effect) CRC is a better choice. CityHash and SpookyHash are curious works in math that have only 1 possible application they can destroy data quickly. The Google Code Search Google Code Search was a free beta product from Google which debuted in Google Labs on October 5, 2006 allowing web users to search for open-source code on the Internet. Google announced that Code Search was to be shut down along with the Code Search API on January 15, 2012.[1] The service remained online until March 2013,[2] and it now returns a 404. If you have a HUGE amount of sensitive data and want to delete it (which will mark the disk sectors free for the OS to use) and DESTROY it so that it could never be recovered by any disk utility tools. There are many free utilities that accomplish this already, but with these new algorithms, the software can accomplish more block corruption in less time. Why does Google Inc invest in and market algorithms with no purpose in any of their active projects? If I was a share-holder, I would vote for new management.

GHash ought not be confused with a block cruncher. The name GHashTableTreeStack was too long so its called GHash. The long name would be more proper, like Mr. Hash. GHash is one single "Data Structure Algorithm" that combines multiple algorithms and data structures (see highlighted) as parts of the whole. GHash uses a Rotating Hash to index a Static Array of Binary Trees. A GBTree is a variant of an automatically balancing AVL Tree that contains an alternate index so that beyond traverse Ascending and Descending (like any B-Tree via Left/Right) it also has a secondary index via Next/Previous which allows the GBTree to also be traversed in the order that items were inserted. The secondary index can optimize certain bulk commits (aka disk writes) of individual updates that were applied while it was stored in RAM by GBTree. The GHash cannot do that, but it uses a GBTree that can. The XML data updates and application layer updates primarily use only the primary index. Upon a special kind of commit, the fragmented memory structure can be flattened back to its initial state into the same order that they came from the disk. Another important algorithmic component of GHash is the unique (aka one of a kind) GHashIterator which allows SIMPLE and non-blocking THREAD-SAFE iteration of this complex data structure via an internal integer index into the Static Array used in combination with a GBTreeIterator which is maintains two GStacks that act as LIFO queues of state information used to quickly iterate the Tree portion of this structure, such a task is typically accomplished by a recursive method of the Tree structure. A mere typical approach will force a multithreaded application to block reads of the structure. GHash is non typical and very fast.

Each algorithm has various properties. Building from a Window64 starting point, I re-structured the Google Sparse HashÂ project source code. I made major structure changes to the code. This may be the answer to a commented question in the source code about an empty bucket. I added struct support for Windows, fully redesigned the build, and provide a 64 bit target for Windows which the Sparse Hash ProjectÂ does not. The work is in the file GSparseHash.h. It will be the file of interest in this release, and admittedly it is still a work in progress. It is a starting point. That one file stands as an exception in the XMLFoundation because it does not yet build under every compiler. It's MUCH simpler to incorporate into any build as the inlined implementation in GSparshHash.h (where the compiler supports) - and I expect only minor changes for GCC to support GSparseHash.h.

Although GHash is not a Distributed Data Structure, it is a far more valuable algorithmic component within a Distributed Data Structure than CityHash. Just as GHash is made of many algorithmic components so must a structure like BigTable designed and used by Google, which is likely where CityHash is used. BigTable is best defined as a sparse, distributed multi-dimensional sorted map.

Boom. Bust. Readjust. Ashes to Ashes. Dust to Dust. In God we Trust. Watch the Bubble sort Bust.

Shrink ▲   
64 bit build results from ExIndexObjects to test XMLObject indexing speeds
----------------------------------------------------
This sample works with HUGE test files.
It is VERY VERY slow under a debugger.
If you dont have enough RAM, or want to speed it up.......
Delete TheWholeTruth.txt as many folks must do in their reality.....
It will use Truth.txt which will obtain evidence you can see.

Note: 777 milliseconds = 777,000 microseconds
Note: The 32 bit build counts cpu clock cycles

[Creating Object Instances]=539 milliseconds
[Create 81 MB XML Document]=1499 milliseconds
[Create 81 MB XML Document Faster]=633 milliseconds
[Save To Disk]=933 milliseconds
[Releasing memory]=1026 milliseconds

--------  GList --------
[InsertObjects]=5472 milliseconds
[Iterate All  ]=821160 objects in 5,245 microseconds
[Search Find  ]=23,455 microseconds
[Update Object]=3,182 microseconds
[Update Faster]=2,982 microseconds
[Iterate All  ]=821160 objs in 4,259 microseconds
[Search NoFind]=33,071 microseconds
[Create XML   ]=661 milliseconds
[XML To Disk  ]=708 milliseconds
[Free Memory  ]=2920 milliseconds
---------------------- Compressed 83,109,379 bytes of XML to 5,131,905

--------  GQSortArray --------
[InsertObjects]=5738 milliseconds
[Iterate All  ]=821160 objects in 3,797 microseconds
[Search Find  ]=1,429 microseconds
[Update Object]=1,113 microseconds
[Update Faster]=1,091 microseconds
[Iterate All  ]=821160 objs in 3,827 microseconds
[Search NoFind]=2,108 microseconds
[Create XML   ]=743 milliseconds
[XML To Disk  ]=1311 milliseconds
[Free Memory  ]=2849 milliseconds
---------------------- Compressed 83,109,379 bytes of XML to 5,131,905

--------  GBTree --------
[InsertObjects]=6814 milliseconds
[Iterate All  ]=821160 objects in 19,073 microseconds
[Search Find  ]=6 microseconds
[Update Object]=45 microseconds
[Update Faster]=29 microseconds
[Iterate All  ]=821160 objs in 18,026 microseconds
[Search NoFind]=6 microseconds
[Create XML   ]=644 milliseconds
[XML To Disk  ]=676 milliseconds
[Free Memory  ]=3844 milliseconds
---------------------- Compressed 83,109,379 bytes of XML to 5,131,905

--------  GHash --------
[InsertObjects]=6061 milliseconds
[Iterate All  ]=821160 objects in 101,138 microseconds
[Search Find  ]=1 microseconds
[Update Object]=38 microseconds
[Update Faster]=29 microseconds
[Iterate All  ]=821160 objs in 111,718 microseconds
[Search NoFind]=1 microseconds
[Create XML   ]=1641 milliseconds
[XML To Disk  ]=745 milliseconds
[Free Memory  ]=4102 milliseconds
---------------------- Compressed 83,109,379 bytes of XML to 6,902,527

--------  GSparseHash --------
[InsertObjects]=6654 milliseconds
[Iterate All  ]=821160 objects in 8,931 microseconds
[Search Find  ]=1 microseconds
[Update Object]=39 microseconds
[Update Faster]=22 microseconds
[Iterate All  ]=821160 objs in 8,705 microseconds
[Search NoFind]=1 microseconds
[Create XML   ]=1659 milliseconds
[XML To Disk  ]=1332 milliseconds
[Free Memory  ]=4352 milliseconds
---------------------- Compressed 83,109,379 bytes of XML to 6,990,074


C:\XMLFoundation\Examples\C++\ExIndexObjects\Release>












CORBA Objects
The XMLFoundation was designed and built for CORBA before it ever added any support for MFC. If you have a pre-existing CORBA system that needs some XML tools, you have come to the right place. If you are building a new CORBA system - this is best tool available for XML support.

If you have read this document all the way to this point, then you will likely understand how the XMLFoundation works for CORBA by showing you this tiny piece of code:

C++
class CustomerImpl : public virtual CustomerBOAImpl, public virtual XMLObject
along with the IMPLEMENT_ORB_FACTORY() macro defined in XMLObject.h, this is how CORBA can natively support the FromXML() and ToXML() by using the XMLFoundation. The Object Factory can instantiate your interface objects for you based on the XML.

CORBA implementations can be done in Java or C++. The XMLFoundation supports both. CORBA breaks down the language barrier allowing Java applications to easily, and natively deal with C++ objects. This example details the creation of C++ CORBA objects - The Java implementation is nearly identical further blurring the lines between Java/C++ within the same project.

The C++ CORBA implementation will bridge into J2EE Application servers everywhere, it will work for any ORB but a few of the most popular ones have been tested, and the makefiles are included with the CORBA sample that ships with the XMLFoundation. The three makefiles included are for:

Borland/Enterprise Studio - Visibroker
IONA/iPortal Enterprise - Orbix
BEA/Weblogic Enterprise - ObjectBroker (works great with Tuxedo implementations)
This example extends the ORB to provide native XML accessors. The sample CORBA application is based around 1 very simple object type. It has a unique integer we call a CustomerID and a string we call a CustomerName. Each customer may contain 0 to n references to another object of the same type as itself, a MyCORBAObject. This would model something like a list of Customers that were referred by 'this' customer.

The IDL Looks Like This
C++
module ExCORBA
{
 interface MyCORBAObject
 {
  void getXMLState(out string s);
  void setXMLState(in string s);
  void setState(in string s, in long l);
  void addSubObject(in string s, in long l);
  void delSubObjects();
  MyCORBAObject getSubObjectIOR(in long l);
  void dumpState(out string s);
 };
};
Follow this 12 Step Program
This is a very simple application. The client application makes 12 calls to the server. Every even numbered call is exactly the same - it is a call to getXMLState() to see what's going on in the server. The client obtains an initial IOR from a server serialized IOR upon server startup.

Step 1 - Assign some state in a native CORBA call. This is a typical CORBA data assignment operation. Two values are set in the object. The client assigns two members on the server. The code looks like this on the client:

C++
CustObject1->setState("Root",777);
Step 2 - Remember - every even numbered call in theis 12 step program is exactly the same

C++
CORBA::String_var s;
CustObject1->getXMLState(s);
Step 2 (behind the scenes) This is how getXMLState() is implemented on the CORBA server:

C++
void ExCORBAImpl::getXMLState( CORBA::String_out s)
{ 
  const char *p = ToXML();
  s = CORBA::string_dup(p);
}
This is the XML sent from the server to the client as the result from step 2.

XML
<MyCORBAImpl>
   <CustomerID>777</CustomerID>
  <CustomerName>Root</CustomerName>
</MyCORBAImpl>
The tag names are configured by the ExCORBAImpl object like this:

C++
void  ExCORBAImpl::MapXMLTagsToMembers()
{
    MapMember(&_nCustID, "CustomerID");
    MapMember(&_strCustName, "CustomerName",&gGenericStrHandler);
    MapMember(&m_lstCMyImplObjs, "MyCORBAImpl",&gGListHandler,0);
}
Step 3 - Update the state of the object through XML. Step 1 used a typical CORBA object accessor to assign the state. Step 3 accomplishes the same through XML. All CORBA objects based on XMLObject support this member assignment style. This is the code on the client:

C++
CustObject1->setXMLState("<MyCorbaImpl><CustomerName>SuperUser</CustomerName></MyCorbaImpl>");
on the server, the implementation is extremely simple - it looks like this:

C++
void ExCORBAImpl::setXMLState( const char* pzXML ) 
{
    FromXML( pzXML );
}
Step 4 - just like step 2 call getXMLState() and this is the result:

XML
<MyCORBAImpl>
   <CustomerID>777</CustomerID>
   <CustomerName>SuperUser</CustomerName>
</MyCORBAImpl>
Step 5 - Add CORBA Sub-Objects through XML. Step 5 is a lot like step 3 where we updated the name "root" to "SuperUser" through an XML assignment. This time, we'll add an object reference. This is a large concept to fully realize and apply - the XML creates CORBA objects.

The client code looks like this:

C++
CustObject1->setXMLState(
 "<MyCORBAImpl>" 
       "<MyCORBAImpl>"
             "<CustomerID>123</CustomerID>" 
             "<CustomerName>Al Gore</CustomerName>" 
       "</MyCORBAImpl>"
       "<MyCORBAImpl>"
             "<CustomerID>456</CustomerID>" 
             "<CustomerName>George Bush Jr.</CustomerName>" 
       "</MyCORBAImpl>"
 "</MyCORBAImpl>");
even though we are creating new CORBA objects we write NO SPECIAL CODE on the server.

We are calling the same implementation on the server we called in Step 3:

C++
void ExCORBAImpl::setXMLState( const char* pzXML ) 
{
   FromXML( pzXML );
}
Step 6 - Just like step 2, call getXMLState() and this is the result:

XML
<MyCORBAImpl>
   <CustomerID>777</CustomerID>
   <CustomerName>SuperUser</CustomerName>
   <MyCORBAImpl>
       <CustomerID>123</CustomerID>
       <CustomerName>Al Gore</CustomerName>
   </MyCORBAImpl>
   <MyCORBAImpl>
       <CustomerID>456</CustomerID>
       <CustomerName>George Bush Jr.</CustomerName>
   </MyCORBAImpl>
</MyCORBAImpl>
Step 7 - Get a CORBA object reference for object instance 456, an object created by the XML. On the client, the code looks like this:

C++
ExCORBA::MyCORBAObject_var CustObject2;
CustObject2 = CustObject1->getSubObjectIOR(456);
and on the server, we walk the list of objects and return the first one that matches the supplied CustomerID like this:

C++
ExCORBA::MyCORBAObject_ptr ExCORBAImpl::getSubObjectIOR(CORBA::Long CustomerID)
{
  // create an iterator[it] for the [m_lstCMyImplObjs] list 
  GListIterator it(&m_lstCMyImplObjs);
  while(it()) // while there is more in the list 
  {
       XMLObject *pO = (XMLObject *)it++; // get the next Customer Interface 
       ExCORBAImpl*pIO = (ExCORBAImpl*)pO->GetInterfaceObject(); // widen the pointer 
       
        // test for a match - Use a keyed datastructure in the real world 
        if (pIO->GetCustomerID() == CustomerID)  
       {
            // Return the CORBA Interface to the desired object 
              return pIO->_this();
            break;
       }
  }
  return 0;
}
Step 8 - Exactly like steps (2, 4, and 6) EXCEPT we are calling toXML() on the Object ref returned by step 7.

XML
<MyCORBAImpl>
  <CustomerID>456</CustomerID>
  <CustomerName>GeorgeBush Jr.</CustomerName>
</MyCORBAImpl>
Step 9 - Add a Sub-Object without using XML. In the same way, we used a traditional member assignment in step 1, we can create a new object reference to demonstrate the two models seamlessly working together. On the client:

C++
CustObject1->addSubObject("Michelangelo",1475);
and on the server, the code looks like this:

C++
void ExCORBAImpl::addSubObject( const char* s, CORBA::Long l )
{
   ExCORBAImpl *p = new ExCORBAImpl;
   p->_nCustID = l;
   p->_strCustName = s;
   m_lstCMyImplObjs.AddLast((XMLObject *)p);
}
Step 10 - Get an object reference to the object created in step 9 and display its state in XML. This is the client code:

C++
CustObject2 = CustObject1->getSubObjectIOR(1475); // exactly like step 7 
CustObject2->getXMLState(s); // like steps (2, 4, 6, and 8) using the new reference.
and the result is:

XML
<MyCORBAImpl>
  <CustomerID>1475</CustomerID>
  <CustomerName>Michelangelo</CustomerName>
</MyCORBAImpl>
Step 11 - Deleting Sub objects. All objects, no matter how they were created, are destroyed the same. The list contains both Factory created objects and Objects created the traditional way. Once again, this shows how seamlessly the ORB fits together with the XMLFoundations's Object Factory. This is the CORBA Implementation/Interface and XMLObject are all one in the same. This cleans up the whole mess.

C++
CustObject1->delSubObjects();
on the server:

C++
void ExCORBAImpl::delSubObjects() IT_THROW_DECL((CORBA::SystemException))
{
   GListIterator it(&m_lstCMyImplObjs);
   while(it())
   {
         XMLObject *pO = (XMLObject *)it++;
         pO->DecRef();
   }
   m_lstCMyImplObjs.RemoveAll();
}
Step 12 - To see that step 11 worked, view the XML state like we did in 2, 4, 6, 8, & 10. Now all the contained objects are gone, and "SuperUser" is alone.

XML
<MyCORBAImpl>
   <CustomerID>777</CustomerID>
   <CustomerName>SuperUser</CustomerName>
</MyCORBAImpl>














