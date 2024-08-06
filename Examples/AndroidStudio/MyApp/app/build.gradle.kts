// [XMLFoundation/examples/Android/MyApp/app/build.gradle.kts]
//
//   **  [MyApp/app]-level ** build file 

plugins{
    alias(libs.plugins.android.application)
}
android{
    namespace = "com.example.myapp"
    compileSdk = 34

    defaultConfig {
        applicationId = "com.example.myapp"
        minSdk = 24
        targetSdk = 34
        versionCode = 1
        versionName = "1.0"
        testInstrumentationRunner = "androidx.test.runner.AndroidJUnitRunner"


        // This adds build flags for all targets that are available to native-lib.cpp and all XMLFoundation library source
	// see native-lib.cpp - 
        externalNativeBuild {
        cmake {
              cppFlags += "-D_USER_DEFINED_CUSTOM_BUILD_FLAG"

        }

        } // end of external native build


    }// end of defaultConfig


    buildTypes {
        release {
            isMinifyEnabled = false
            ndk {
                isDebuggable = false
                debugSymbolLevel = "FULL"
            }

            proguardFiles(
                getDefaultProguardFile("proguard-android-optimize.txt"),
                "proguard-rules.pro"
            )
        }
        debug {
            ndk {
                isDebuggable = true
                debugSymbolLevel = "FULL"
            }
    //      ext.enableCrashlytics = false
    //      ext.set()
            proguardFiles(
                getDefaultProguardFile("proguard-android-optimize.txt"),
                "proguard-rules.pro"
            )
        }
   }

    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_1_8
        targetCompatibility = JavaVersion.VERSION_1_8
    }

    externalNativeBuild {
         cmake {
             path = file("src/main/cpp/CMakeLists.txt")
             version = "3.22.1"
         }
    }

    // other buildFeaturs
    //        compose = true
    //        buildConfig = true
    buildFeatures {
        viewBinding = true
    }
}

dependencies{

    implementation(libs.appcompat)
    implementation(libs.material)
    implementation(libs.constraintlayout)
    testImplementation(libs.junit)
    androidTestImplementation(libs.ext.junit)
    androidTestImplementation(libs.espresso.core)
}
