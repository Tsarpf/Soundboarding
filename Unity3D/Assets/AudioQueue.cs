using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

struct AudioChunk
{
    int count;
}
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
        float sampleFloat = sample / bitDepth * 2 - 1;
        audio.Add(sampleFloat);
    }

    public bool Empty()
    {
        return position < audio.Count;
    }

    public int DataLeft()
    {
        return audio.Count - position;
    }

    public float Dequeue()
    {
        if (Empty())
        {
            return 0;
        }
        return audio[position++];
    }
    
}
