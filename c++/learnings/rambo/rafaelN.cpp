//Finally a simple OpenAL example program.
//Gordon Griesel
//2015 - 2018
//edited by: Rafael Noriega
//Spring 2018
#include <iostream>
#include "Global.h"
#include "Timers.h"
#include "SpriteSheet.h"
#include "Character.h"
#include <GL/glx.h>
#include <sstream>
#include "fonts.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <fstream>
#include <fcntl.h>
#include <sys/stat.h>
#include "rafaelN.h"
#include <stdlib.h>
#ifdef USE_OPENAL_SOUND
#include </usr/include/AL/alut.h>
#include <pthread.h>
#endif //USE_OPENAL_SOUND

bool inUse[8] = {0,0,0,0,0,0,0,0};
bool flag2 = true;
extern Global g;
extern std::string player_name;

void* shoot(void* arg)
{
	if (arg){

	}
	//int* threadNumber = (int*)arg;
	//int i = *threadNumber;

	//printf("Thread number: %i\n",i);	

	//Get started right here.
#ifdef USE_OPENAL_SOUND
	//	alutInit(0, NULL);
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR: alutInit()\n");
		//return 0;
	}
	//Clear error state.
	alGetError();
	//
	//Setup the listener.
	//Forward and up vectors are used.
	float vec[6] = {0.0f,0.0f,1.0f, 0.0f,1.0f,0.0f};
	alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
	alListenerfv(AL_ORIENTATION, vec);
	alListenerf(AL_GAIN, 1.0f);
	//
	//Buffer holds the sound information.
	ALuint alBuffer;
	alBuffer = alutCreateBufferFromFile("sound/gun2.wav");
	//Source refers to the sound.
	ALuint alSource;
	//Generate a source, and store it in a buffer.
	alGenSources(1, &alSource);
	alSourcei(alSource, AL_BUFFER, alBuffer);
	//Set volume and pitch to normal, no looping of sound.
	alSourcef(alSource, AL_GAIN, 1.0f);
	alSourcef(alSource, AL_PITCH, 1.0f);
	alSourcei(alSource, AL_LOOPING, AL_FALSE);
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR: setting source\n");
		alutExit();
		//   inUse[i] = 0;
		pthread_exit(NULL);
		//return 0;
	}
	ALint state;
	alSourcePlay(alSource);
	do {
		alGetSourcei(alSource, AL_SOURCE_STATE, &state);
	} while (state == AL_PLAYING);
	//alSourceStop(alSource); 
	//  usleep(250000);
	//Cleanup.
	//First delete the source.
	alDeleteSources(1, &alSource);
	//Delete the buffer.
	alDeleteBuffers(1, &alBuffer);
#endif //USE_OPENAL_SOUND
	pthread_exit(NULL);
	//return 0;
}

void* nuke(void* arg)
{
	if (arg){

	}
	//int* threadNumber = (int*)arg;
	//int i = *threadNumber;

	//printf("Thread number: %i\n",i);	

	//Get started right here.
#ifdef USE_OPENAL_SOUND
	//	alutInit(0, NULL);
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR: alutInit()\n");
		//return 0;
	}
	//Clear error state.
	alGetError();
	//
	//Setup the listener.
	//Forward and up vectors are used.
	float vec[6] = {0.0f,0.0f,1.0f, 0.0f,1.0f,0.0f};
	alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
	alListenerfv(AL_ORIENTATION, vec);
	alListenerf(AL_GAIN, 1.0f);
	//
	//Buffer holds the sound information.
	ALuint alBuffer;
	alBuffer = alutCreateBufferFromFile("sound/bomb.wav");
	//Source refers to the sound.
	ALuint alSource;
	//Generate a source, and store it in a buffer.
	alGenSources(1, &alSource);
	alSourcei(alSource, AL_BUFFER, alBuffer);
	//Set volume and pitch to normal, no looping of sound.
	alSourcef(alSource, AL_GAIN, 1.0f);
	alSourcef(alSource, AL_PITCH, 1.0f);
	alSourcei(alSource, AL_LOOPING, AL_FALSE);
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR: setting source\n");
		alutExit();
		//   inUse[i] = 0;
		pthread_exit(NULL);
		//return 0;
	}
	ALint state;
	alSourcePlay(alSource);
	do {
		alGetSourcei(alSource, AL_SOURCE_STATE, &state);
	} while (state == AL_PLAYING);
	//alSourceStop(alSource); 
	//  usleep(250000);
	//Cleanup.
	//First delete the source.
	alDeleteSources(1, &alSource);
	//Delete the buffer.
	alDeleteBuffers(1, &alBuffer);
#endif //USE_OPENAL_SOUND
	pthread_exit(NULL);
}

void* hit(void* arg)
{
	if (arg){

	}
	//int* threadNumber = (int*)arg;
	//int i = *threadNumber;

	//printf("Thread number: %i\n",i);	

	//Get started right here.
#ifdef USE_OPENAL_SOUND
	//	alutInit(0, NULL);
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR: alutInit()\n");
		//return 0;
	}
	//Clear error state.
	alGetError();
	//
	//Setup the listener.
	//Forward and up vectors are used.
	float vec[6] = {0.0f,0.0f,1.0f, 0.0f,1.0f,0.0f};
	alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
	alListenerfv(AL_ORIENTATION, vec);
	alListenerf(AL_GAIN, 1.0f);
	//
	//Buffer holds the sound information.
	ALuint alBuffer;
	alBuffer = alutCreateBufferFromFile("sound/hit.wav");
	//Source refers to the sound.
	ALuint alSource;
	//Generate a source, and store it in a buffer.
	alGenSources(1, &alSource);
	alSourcei(alSource, AL_BUFFER, alBuffer);
	//Set volume and pitch to normal, no looping of sound.
	alSourcef(alSource, AL_GAIN, 1.0f);
	alSourcef(alSource, AL_PITCH, 1.0f);
	alSourcei(alSource, AL_LOOPING, AL_FALSE);
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR: setting source\n");
		alutExit();
		//   inUse[i] = 0;
		pthread_exit(NULL);
		//return 0;
	}
	ALint state;
	alSourcePlay(alSource);
	do {
		alGetSourcei(alSource, AL_SOURCE_STATE, &state);
	} while (state == AL_PLAYING);
	//alSourceStop(alSource); 
	//  usleep(250000);
	//Cleanup.
	//First delete the source.
	alDeleteSources(1, &alSource);
	//Delete the buffer.
	alDeleteBuffers(1, &alBuffer);
#endif //USE_OPENAL_SOUND
	pthread_exit(NULL);
}

void* bg_sound(void* arg)
{
	if (arg){

	}
	//int* threadNumber = (int*)arg;
	//int i = *threadNumber;

	//printf("Thread number: %i\n",i);	

	//Get started right here.
#ifdef USE_OPENAL_SOUND
	//	alutInit(0, NULL);
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR: alutInit()\n");
		//return 0;
	}
	//Clear error state.
	alGetError();
	//
	//Setup the listener.
	//Forward and up vectors are used.
	float vec[6] = {0.0f,0.0f,1.0f, 0.0f,1.0f,0.0f};
	alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
	alListenerfv(AL_ORIENTATION, vec);
	alListenerf(AL_GAIN, 1.0f);
	//
	//Buffer holds the sound information.
	ALuint alBuffer;
	alBuffer = alutCreateBufferFromFile("sound/menu_battle.wav");
	//Source refers to the sound.
	ALuint alSource;
	//Generate a source, and store it in a buffer.
	alGenSources(1, &alSource);
	alSourcei(alSource, AL_BUFFER, alBuffer);
	//Set volume and pitch to normal, no looping of sound.
	alSourcef(alSource, AL_GAIN, 1.0f);
	alSourcef(alSource, AL_PITCH, 1.0f);
	alSourcei(alSource, AL_LOOPING, AL_FALSE);
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR: setting source\n");
		alutExit();
		//   inUse[i] = 0;
		pthread_exit(NULL);
		//return 0;
	}
	ALint state;
	alSourcePlay(alSource);
	do {
		alGetSourcei(alSource, AL_SOURCE_STATE, &state);
	} while (state == AL_PLAYING);
	//alSourceStop(alSource); 
	//  usleep(250000);
	//Cleanup.
	//First delete the source.
	alDeleteSources(1, &alSource);
	//Delete the buffer.
	alDeleteBuffers(1, &alBuffer);
#endif //USE_OPENAL_SOUND
	pthread_exit(NULL);
	//return 0;
}

void* life(void* arg)
{
	if (arg){

	}
	//int* threadNumber = (int*)arg;
	//int i = *threadNumber;

	//printf("Thread number: %i\n",i);	

	//Get started right here.
#ifdef USE_OPENAL_SOUND
	//	alutInit(0, NULL);
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR: alutInit()\n");
		//return 0;
	}
	//Clear error state.
	alGetError();
	//
	//Setup the listener.
	//Forward and up vectors are used.
	float vec[6] = {0.0f,0.0f,1.0f, 0.0f,1.0f,0.0f};
	alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
	alListenerfv(AL_ORIENTATION, vec);
	alListenerf(AL_GAIN, 1.0f);
	//
	//Buffer holds the sound information.
	ALuint alBuffer;
	alBuffer = alutCreateBufferFromFile("sound/life.wav");
	//Source refers to the sound.
	ALuint alSource;
	//Generate a source, and store it in a buffer.
	alGenSources(1, &alSource);
	alSourcei(alSource, AL_BUFFER, alBuffer);
	//Set volume and pitch to normal, no looping of sound.
	alSourcef(alSource, AL_GAIN, 1.0f);
	alSourcef(alSource, AL_PITCH, 1.0f);
	alSourcei(alSource, AL_LOOPING, AL_FALSE);
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR: setting source\n");
		alutExit();
		//   inUse[i] = 0;
		pthread_exit(NULL);
		//return 0;
	}
	ALint state;
	alSourcePlay(alSource);
	do {
		alGetSourcei(alSource, AL_SOURCE_STATE, &state);
	} while (state == AL_PLAYING);
	//alSourceStop(alSource); 
	//  usleep(250000);
	//Cleanup.
	//First delete the source.
	alDeleteSources(1, &alSource);
	//Delete the buffer.
	alDeleteBuffers(1, &alBuffer);
#endif //USE_OPENAL_SOUND
	pthread_exit(NULL);
	//return 0;
}

void* fire_speed(void* arg)
{
	if (arg){

	}
	//int* threadNumber = (int*)arg;
	//int i = *threadNumber;

	//printf("Thread number: %i\n",i);	

	//Get started right here.
#ifdef USE_OPENAL_SOUND
	//	alutInit(0, NULL);
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR: alutInit()\n");
		//return 0;
	}
	//Clear error state.
	alGetError();
	//
	//Setup the listener.
	//Forward and up vectors are used.
	float vec[6] = {0.0f,0.0f,1.0f, 0.0f,1.0f,0.0f};
	alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
	alListenerfv(AL_ORIENTATION, vec);
	alListenerf(AL_GAIN, 1.0f);
	//
	//Buffer holds the sound information.
	ALuint alBuffer;
	alBuffer = alutCreateBufferFromFile("sound/fire_speed.wav");
	//Source refers to the sound.
	ALuint alSource;
	//Generate a source, and store it in a buffer.
	alGenSources(1, &alSource);
	alSourcei(alSource, AL_BUFFER, alBuffer);
	//Set volume and pitch to normal, no looping of sound.
	alSourcef(alSource, AL_GAIN, 1.0f);
	alSourcef(alSource, AL_PITCH, 1.0f);
	alSourcei(alSource, AL_LOOPING, AL_FALSE);
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR: setting source\n");
		alutExit();
		//   inUse[i] = 0;
		pthread_exit(NULL);
		//return 0;
	}
	ALint state;
	alSourcePlay(alSource);
	do {
		alGetSourcei(alSource, AL_SOURCE_STATE, &state);
	} while (state == AL_PLAYING);
	//alSourceStop(alSource); 
	//  usleep(250000);
	//Cleanup.
	//First delete the source.
	alDeleteSources(1, &alSource);
	//Delete the buffer.
	alDeleteBuffers(1, &alBuffer);
#endif //USE_OPENAL_SOUND
	pthread_exit(NULL);
	//return 0;
}

void sound(int select) {
    pthread_t thread;
    //for(int i=1; i< 8; i++) {
    //if (!inUse[i]) {
    // inUse[i] = 1;
    switch(select)
    {
	//sound for the main menu war background sound
	case 1:
	    if (flag2){ 
		if (pthread_create(&thread, NULL, bg_sound, (void*)NULL)) {	
		    printf("Error: unable to create thread\n");
		} else {
		    //printf("Thread created\n");
		}
		flag2 = false;
	    }
	    return;
	    //sound for the gunshots
	case 2: if (pthread_create(&thread, NULL, shoot, (void*)NULL)) {
		    printf("Error: unable to create thread\n");
		} else {
		    //printf("Thread created for shooting\n");
		    return;
		}
		//sound for when an enemy hits rambo
	case 3: if (pthread_create(&thread, NULL, hit, (void*)NULL)) {
		    printf("Error: unable to create thread\n");
		} else {
		    //printf("Thread created for hit\n");
		    return;
		}
		//sound for the nuke powerup
	case 4: if (pthread_create(&thread, NULL, nuke, (void*)NULL)) {
		    printf("Error: unable to create thread\n");
		} else {
		    //printf("Thread created for nuke\n");
		    return;
		}
		//sound for gaining life	
	case 5: if (pthread_create(&thread, NULL, life, (void*)NULL)) {
		    printf("Error: unable to create thread\n");
		} else {
		    //printf("Thread created for nuke\n");
		    return;
		}
		//sound for shooting faster	
	case 6: if (pthread_create(&thread, NULL, fire_speed, (void*)NULL)) {
		    printf("Error: unable to create thread\n");
		} else {
		    //printf("Thread created for nuke\n");
		    return;
		}
    }    
}


void leader_board()
{
    //printf("In leaderboard\n");

    std::stringstream ss;
    ss<<g.score;
    std::string score_str = ss.str();
    printf("*********************************\n");  
    printf("Congrats, your score was %s\n", score_str.c_str());
    printf("*********************************\n");
    //printf("Enter name for leaderboard: ");

	std::string command = 
		"curl -i http://cs.csubak.edu/\\~rnoriega/3350/leaderboard/update_scores.php";
	command += "\\?name=" + player_name;
	command += "\\&score=" + score_str;

    system(command.c_str());
    //printf("exit leader_board()\n");
}
