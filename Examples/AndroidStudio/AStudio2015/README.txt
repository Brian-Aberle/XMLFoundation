






  *****  Everything under the AStudio2015 folder remains in the source distro for reference
  *****  because the source code remains helpful although theses projects no longer build
  *****  under the Android Studio 2024.  The only thing "wrong" with them is the project/build 
  *****  configurations that will eventually be updated to the current format/syntax
  *****  The source remains helpful until then - most specifically 
  *****          ObjectModel.java  within GApp.cpp using the JavaXMLFoundation
  *****    and   GApp.cpp with general XMLFoundation library usage 











1st You need to open the file [XMLFoundation/Examples/Android/XMLFoundationProject/XMLFoundationBuildPath.properties]

		XMLFoundation.dir=C:\\XMLFoundation
				or
		XMLFoundation.dir=/home/user/XMLFoundation


next you need to open 2 files,  [XMLFoundation/Examples/Android/XMLFoundationProject/GApp/Build.gradle] and 
				[XMLFoundation/Examples/Android/XMLFoundationProject/XMLFLibrary/build.gradle]
to setup your local build include paths to XMLFoundation 

search for:
// SETUP-YOUR-BUILD

            cppFlags.add('-I/home/user/XMLFoundation/Libraries/XMLFoundation/inc')
            cppFlags.add('-I/home/user/XMLFoundation/Libraries/XMLFoundation/src')
            cppFlags.add('-I/home/user/XMLFoundation/Servers/Core')
            cppFlags.add('-I/home/user/XMLFoundation/Libraries/openssl/bin-android-x86')

//          cppFlags.add('-IC:\\XMLFoundation/Libraries/XMLFoundation/inc')
//          cppFlags.add('-IC:\\XMLFoundation/Libraries/XMLFoundation/src')
//          cppFlags.add('-IC:\\XMLFoundation/Servers/Core')
//          cppFlags.add('-IC:\\XMLFoundation\\Libraries\\openssl\\bin-android-armeabi-v7a')




then... Open XMLFoundationProject in the Graphical IDE



