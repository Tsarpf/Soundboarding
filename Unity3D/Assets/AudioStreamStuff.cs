using UnityEngine;
using System.Collections;
using System.IO;
using System.Net;
using System.Net.Sockets;


public class AudioStreamStuff : MonoBehaviour
{
    public int position = 0;
    public int sampleRate = 0;
    public float frequency = 440;
    AudioQueue queue;
    StreamReader audioReader;
    TcpClient client;
    NetworkStream stream;
    void Start() {
        queue = new AudioQueue();
        
        client = new TcpClient("localhost", 3666);
        stream = client.GetStream();

        audioReader = new StreamReader(stream);
        AudioClip myClip = AudioClip.Create("spotifru", 44100 * 10000, 2, 44100, false, true, GetAudioFromQueue);
        audio.clip = myClip;
        //AudioClip myClip = AudioClip.Create("MySinoid", 44100, 1, 44100, false, true, OnAudioRead, OnAudioSetPosition);
        //sampleRate = AudioSettings.outputSampleRate;
        //audio.clip = myClip;
        audio.PlayDelayed(2); ;
    }
    /*
    void OnAudioRead(float[] data) {
        int count = 0;
        while (count < data.Length) {
            data[count] = Mathf.Sign(Mathf.Sin(2 * Mathf.PI * frequency * position / sampleRate));
            position++;
            count++;
        }
    }
    */

    int readPerUpdate = 44100 / 2;
    bool start = true;
    void Update()
    {
        GetAudioFromServer(readPerUpdate);
        /*
        if (start)
        {
        }
        start = false;
        */
    }

    void GetAudioFromServer(int sampleCount)
    {
        byte[] inbuffer = new byte[2];

        if (stream.CanRead)
        {
            int i = 0;
            while (stream.DataAvailable && i < sampleCount)
            {
                int bytesRead = stream.Read(inbuffer, 0, inbuffer.Length);
                ushort result = System.BitConverter.ToUInt16(inbuffer, 0);
                queue.Enqueue(result);
                i++;
            }
        }
    }

    void GetAudioFromQueue(float[] data)
    {
        if (queue.DataLeft() < data.Length)
        {
            Debug.Log("Warning: not enough audio, filling rest with zeroes");
            int j = 0;
            for (int i = 0; i < queue.DataLeft(); i++)
            {
                data[i] = queue.Dequeue();
                j++;
            }
            for (; j < data.Length; j++)
            {
                data[j] = 0;
            }
            return;
        }

        for (int i = 0; i < data.Length; i++)
        {
            data[i] = queue.Dequeue();
        }
    }
    void OnAudioSetPosition(int newPosition)
    {
        position = newPosition;
    }
}