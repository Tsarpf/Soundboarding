using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
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




    public void Enqueue(int sample)
    {
        float sampleFloat = (float)sample / (float)bitDepth * 2 - 1;
        //float sampleFloat = (float)sample - bitDepth / 2;
        
        
        audio.Add(sampleFloat);
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
    public float[] DequeueMany()
    {

    }
    */
    
}
