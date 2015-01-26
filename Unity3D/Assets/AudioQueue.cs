using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;
class AudioQueue
{
    int position = 0;
    int bitDepth;

    List<float> audio;
    public AudioQueue()
    {
        bitDepth = (int)Math.Pow(2,16);
        audio = new List<float>();
    }




    int lastSec = 0;
    public void Enqueue(short sample)
    {
        //float sampleFloat = (float)sample / (float)bitDepth; //* 2 - 1;
        //float sampleFloat = ((float)sample + ((float)bitDepth / 2.0f)) / 2.0f;
        //sampleFloat /= bitDepth;
        //float sampleFloat = (float)sample - bitDepth / 2;

       //float sampleFloat = ((float)sample + ((float)bitDepth / 2.0f)) / (float)bitDepth; //0-1
        
        //float sampleFloat = (float)sample / ((float)bitDepth / 2.0f);

        float sampleFloat = (float)(sample + bitDepth) / (float)(2 * bitDepth);
        
        audio.Add(sampleFloat);

        int seconds = DataLeft() / 44100 / 2;
        if (seconds > lastSec)
        {
            lastSec = seconds;
            Debug.Log("Seconds: " + seconds);
        }
    }

    public bool Empty()
    {
        return position >= audio.Count;
    }

    public int DataLeft()
    {
        return audio.Count - position;
    }

    public float Dequeue()
    {
        return audio[position++];
    }

    /*
    public float[] DequeueMany(int count)
    {
        float[] ret = new float[count];
        Buffer.BlockCopy(audio.ToArray(), 0, ret, 0, count);
        return ret;
    }
    */
    
}
