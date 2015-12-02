// WavWriter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#define M_PI 3.14159265

unsigned long int subChunk2Size = 0;
fstream stream;
int sampleRate, lastSample = 0;

void put_int4(long unsigned int value)
{
	unsigned int a = value % 256;
	stream.put(a);
	value -= a;
	if(value <= 0)
	{
		stream.put(0);
		stream.put(0);
		stream.put(0);
		return;
	}
	a = (value % 65536) / 256;
	stream.put(a);
	value -= a;
	if(value <= 0)
	{
		stream.put(0);
		stream.put(0);
		return;
	}
	a = (value % 16777216) / 65536;
	stream.put(a);
	value -= a;
	if(value <= 0)
	{
		stream.put(0);
		return;
	}
	a = value / 4294967296;
	stream.put(a);
}
void put_int2(int value)
{
	stream.put( value % 256 );
	stream.put( value / 256 );
}
void put_signed_int2(int value)
{
	if(value < 0)
	{
		value += 65536;
	}
	stream.put( value % 256 );
	stream.put( value / 256 );
}

void put_sample(int value)
{
	put_signed_int2(value);
	subChunk2Size += 2;
}

void put_header(int numChannels, int Samplerate)
{
	stream.seekp(0);
	stream.write("RIFF", 4);
	
	stream.seekp(8);
	stream.write("WAVEfmt ", 8);

	put_int4(16);
	put_int2(1);
	put_int2(numChannels);//Number of channels
	put_int4(Samplerate);//Samplerate, littleEndian
	put_int4(Samplerate * numChannels * 2);//ByteRate = SampleRate * NumChannels * BitsPerSample/8
	put_int2(numChannels * 2);//BlockAlign = NumChannels * BitsPerSample/8
	put_int2(16);//BitsPerSample

	stream.write("data", 4);
}
void put_sizes()
{
	stream.seekp(4);
	put_int4(subChunk2Size + 36);
	stream.seekg(40);
	put_int4(subChunk2Size);
}

enum Pitch {
C4 = 131,
Cs4 = 139,
Db4 = 139,
D4 = 147,
Ds4 = 156,
Eb4 = 156,
E4 = 165,
F4 = 175,
Fs4 = 185,
Gb4	= 185,
G4 = 196,
Gs4 = 208,
Ab4	= 208,
A4 = 220,
As4 = 223,
Bb4	= 233,
B4 = 247,
C5 = 262,
Cs5 = 277,
Db5 = 277,
D5 = 294,
Ds5 = 311,
Eb5	= 311,
E5 = 330,
F5 = 349,
Fs5 = 370,
GB5	= 370,
G5 = 392,
Gs5 = 415,
Ab5	= 415,
A5 = 440,
As5 = 466,
Bb5	= 466,
B5 = 494,
C6 = 523 };
enum Length {
	w = 2000,
	hd = 1500,
	h = 1000,
	qd = 666,
	q = 500,
	ed = 333,
	e = 250,
	trip =167,
	s = 125};

void n(int p, int l)
{
	int nOfSamples = (float)sampleRate * ((float)l / 1000);
	for(int s = 0; s < nOfSamples; s++)
	{
		lastSample = (32768) * sin(s * 4 * M_PI * p / sampleRate);
		put_sample(lastSample);
	}
}
void r(int l)
{
	int nOfSamples = (float)sampleRate * ((float)l / 1000);
	for(int s = 0; s < nOfSamples; s++)
	{
		put_sample(lastSample);
	}
}

void put_data()
{
	n(E5, s);//1
	n(E5, s);
	r(s);
	n(E5, s);
	r(s);
	n(C5, s);
	n(E5, s);
	r(s);
	n(G5, q);
	n(G4, q);

	n(C5, s);//2
	r(e);
	n(G4, s);
	r(e);
	n(E4, s);
	r(e);
	n(A4, s);
	r(s);
	n(B4, s);
	r(s);
	n(As4, s);
	n(A4, s);
	r(s);

	n(G4, trip);//3
	n(E5, trip);
	n(G5, trip);
	n(A5, s);
	r(s);
	n(F5, s);
	n(G5, s);
	n(G5, s);
	n(E5, s);
	r(s);
	n(C5, s);
	n(D5, s);
	n(B4, s);
	r(e);

	n(C5, s);//4
	r(e);
	n(G4, s);
	r(e);
	n(E4, s);
	r(e);
	n(A4, s);
	r(s);
	n(B4, s);
	r(s);
	n(As4, s);
	n(A4, s);
	r(s);
	
	n(G4, trip);//5
	n(E5, trip);
	n(G5, trip);
	n(A5, s);
	r(s);
	n(F5, s);
	n(G5, s);
	n(G5, s);
	n(E5, s);
	r(s);
	n(C5, s);
	n(D5, s);
	n(B4, s);
	r(e);

	r(e);//6
	n(G5, s);
	n(Fs5, s);
	n(F5, s);
	n(D5, s);
	r(s);
	n(E5, s);
	r(s);
	n(Gs4, s);
	n(E4, s);
	n(C5, s);
	r(s);
	n(E4, s);
	n(C5, s);
	n(D5, s);

	r(e);//7
	n(G5, s);
	n(Fs5, s);
	n(F5, s);
	n(D5, s);
	r(s);
	n(E5, s);
	r(s);
	n(C6, s);
	r(s);
	n(C6, s);
	n(C6, s);
	r(s);
	r(e);

	r(e);//8
	n(G5, s);
	n(Fs5, s);
	n(F5, s);
	n(D5, s);
	r(s);
	n(E5, s);
	r(s);
	n(Gs4, s);
	n(E4, s);
	n(C5, s);
	r(s);
	n(E4, s);
	n(C5, s);
	n(D5, s);

	r(e);//9
	n(Eb5, s);
	r(e);
	n(D5, s);
	r(e);
	n(C5, s);
	r(s);
	r(e);
	r(q);

	r(e);//10
	n(G5, s);
	n(Fs5, s);
	n(F5, s);
	n(D5, s);
	r(s);
	n(E5, s);
	r(s);
	n(Gs4, s);
	n(E4, s);
	n(C5, s);
	r(s);
	n(E4, s);
	n(C5, s);
	n(D5, s);

	r(e);//11
	n(G5, s);
	n(Fs5, s);
	n(F5, s);
	n(D5, s);
	r(s);
	n(E5, s);
	r(s);
	n(C6, s);
	r(s);
	n(C6, s);
	n(C6, s);
	r(s);
	r(e);

	r(e);//12
	n(G5, s);
	n(Fs5, s);
	n(F5, s);
	n(D5, s);
	r(s);
	n(E5, s);
	r(s);
	n(Gs4, s);
	n(E4, s);
	n(C5, s);
	r(s);
	n(E4, s);
	n(C5, s);
	n(D5, s);

	r(e);//13
	n(Eb5, s);
	r(e);
	n(D5, s);
	r(e);
	n(C5, s);
	r(s);
	r(e);
	r(q);

	n(C5, s);//14
	n(C5, s);
	r(s);
	n(C5, s);
	r(s);
	n(C5, s);
	n(D5, s);
	r(s);
	n(E5, s);
	n(C5, s);
	r(s);
	n(A4, s);
	n(G4, s);
	r(s);
	r(e);

	n(C5, s);//15
	n(C5, s);
	r(s);
	n(C5, s);
	r(s);
	n(C5, s);
	n(D5, s);
	n(E5, s);
	r(h);

	n(C5, s);//16
	n(C5, s);
	r(s);
	n(C5, s);
	r(s);
	n(C5, s);
	n(D5, s);
	r(s);
	n(E5, s);
	n(C5, s);
	r(s);
	n(A4, s);
	n(G4, s);
	r(s);
	r(e);

	n(E5, s);//17
	n(E5, s);
	r(s);
	n(E5, s);
	r(s);
	n(C5, s);
	n(E5, s);
	r(s);
	n(G5, q);
	n(G4, q);

	n(C5, s);//18
	r(e);
	n(G4, s);
	r(e);
	n(E4, s);
	r(e);
	n(A4, s);
	r(s);
	n(B4, s);
	r(s);
	n(As4, s);
	n(A4, s);
	r(s);

	n(G4, trip);//19
	n(E5, trip);
	n(G5, trip);
	n(A5, s);
	r(s);
	n(F5, s);
	n(G5, s);
	n(G5, s);
	n(E5, s);
	r(s);
	n(C5, s);
	n(D5, s);
	n(B4, s);
	r(e);

	n(C5, s);//20
	r(e);
	n(G4, s);
	r(e);
	n(E4, s);
	r(e);
	n(A4, s);
	r(s);
	n(B4, s);
	r(s);
	n(As4, s);
	n(A4, s);
	r(s);

	n(G4, trip);//21
	n(E5, trip);
	n(G5, trip);
	n(A5, s);
	r(s);
	n(F5, s);
	n(G5, s);
	n(G5, s);
	n(E5, s);
	r(s);
	n(C5, s);
	n(D5, s);
	n(B4, s);
	r(e);

	n(E5, s);//22
	n(C5, s);
	r(s);
	n(G4, s);
	r(e);
	n(Gs4, s);
	r(s);
	n(A4, s);
	n(F5, s);
	r(s);
	n(F5, s);
	n(A4, s);
	r(s);
	r(e);

	n(B4, trip);//23
	n(A5, trip);
	n(A5, trip);
	n(A5, trip);
	n(G5, trip);
	n(F5, trip);
	n(E5, s);
	n(C5, s);
	r(s);
	n(A4, s);
	n(G4, s);
	r(s);
	r(e);

	n(E5, s);//24
	n(C5, s);
	r(s);
	n(G4, s);
	r(e);
	n(Gs4, s);
	r(s);
	n(A4, s);
	n(F5, s);
	r(s);
	n(F5, s);
	n(A4, s);
	r(s);
	r(e);

	n(B4, s);//25
	n(F5, s);
	r(s);
	n(F5, s);
	n(F5, trip);
	n(E5, trip);
	n(D5, trip);
	n(C5, s);
	n(E4, s);
	r(s);
	n(E4, s);
	n(C4, s);
	r(s);
	r(e);

	n(E5, s);//22-2
	n(C5, s);
	r(s);
	n(G4, s);
	r(e);
	n(Gs4, s);
	r(s);
	n(A4, s);
	n(F5, s);
	r(s);
	n(F5, s);
	n(A4, s);
	r(s);
	r(e);

	n(B4, trip);//23-2
	n(A5, trip);
	n(A5, trip);
	n(A5, trip);
	n(G5, trip);
	n(F5, trip);
	n(E5, s);
	n(C5, s);
	r(s);
	n(A4, s);
	n(G4, s);
	r(s);
	r(e);

	n(E5, s);//24-2
	n(C5, s);
	r(s);
	n(G4, s);
	r(e);
	n(Gs4, s);
	r(s);
	n(A4, s);
	n(F5, s);
	r(s);
	n(F5, s);
	n(A4, s);
	r(s);
	r(e);

	n(B4, s);//25-2
	n(F5, s);
	r(s);
	n(F5, s);
	n(F5, trip);
	n(E5, trip);
	n(D5, trip);
	n(C5, s);
	n(E4, s);
	r(s);
	n(E4, s);
	n(C4, s);
	r(s);
	r(e);

	n(C5, s);//26
	n(C5, s);
	r(s);
	n(C5, s);
	r(s);
	n(C5, s);
	n(D5, s);
	r(s);
	n(E5, s);
	n(C5, s);
	r(s);
	n(A4, s);
	n(G4, s);
	r(s);
	r(e);

	n(C5, s);//27
	n(C5, s);
	r(s);
	n(C5, s);
	r(s);
	n(C5, s);
	n(D5, s);
	n(E5, s);
	r(h);

	n(C5, s);//28
	n(C5, s);
	r(s);
	n(C5, s);
	r(s);
	n(C5, s);
	n(D5, s);
	r(s);
	n(E5, s);
	n(C5, s);
	r(s);
	n(A4, s);
	n(G4, s);
	r(s);
	r(e);

	n(E5, s);//29
	n(E5, s);
	r(s);
	n(E5, s);
	r(s);
	n(C5, s);
	n(E5, s);
	r(s);
	n(G5, q);
	n(G4, q);

	n(E5, s);//30
	n(C5, s);
	r(s);
	n(G4, s);
	r(e);
	n(Gs4, s);
	r(s);
	n(A4, s);
	n(F5, s);
	r(s);
	n(F5, s);
	n(A4, s);
	r(s);
	r(e);

	n(B4, trip);//31
	n(A5, trip);
	n(A5, trip);
	n(A5, trip);
	n(G5, trip);
	n(F5, trip);
	n(E5, s);
	n(C5, s);
	r(s);
	n(A4, s);
	n(G4, s);
	r(s);
	r(e);

	n(E5, s);//32
	n(C5, s);
	r(s);
	n(G4, s);
	r(e);
	n(Gs4, s);
	r(s);
	n(A4, s);
	n(F5, s);
	r(s);
	n(F5, s);
	n(A4, s);
	r(s);
	r(e);

	n(B4, s);//33
	n(F5, s);
	r(s);
	n(F5, s);
	n(F5, trip);
	n(E5, trip);
	n(D5, trip);
	n(C5, s);
	n(E4, s);
	r(s);
	n(E4, s);
	n(C4, s);
	r(s);
	r(e);

	n(C5, s);//34
	r(e);
	n(G4, s);
	r(e);
	n(E4, s);
	r(s);
	n(A4, trip);
	n(B4, trip);
	n(A4, trip);
	n(Ab4, trip);
	n(Bb4, trip);
	n(Ab4, e);
	n(G4, e);
	n(F4, e);
	n(G4, qd);
}

int _tmain(int argc, _TCHAR* argv[])
{
	stream.open("test.wav",ios_base::out | ios::binary);
	if(stream.is_open())
	{
		sampleRate = 22050;
		put_header(1, sampleRate);
		put_data();
		put_sizes();
	}
	stream.close();
	//system("PAUSE");
	return 0;
}

