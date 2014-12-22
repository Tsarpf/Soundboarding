using UnityEngine;
using System.Collections;
using System.IO;
using System.Net;
using System.Net.Sockets;
//using System.Diagnostics;


public class AudioStreamStuff : MonoBehaviour
{
    //public int position = 0;
    //public int sampleRate = 0;
    //public float frequency = 440;
    AudioQueue queue;
    StreamReader audioReader;
    TcpClient client;
    NetworkStream stream;
    void Awake()
    {
        
        queue = new AudioQueue();
        
        client = new TcpClient("localhost", 3666);
        stream = client.GetStream();

        audioReader = new StreamReader(stream);
        //AudioClip myClip = AudioClip.Create("MySinoid", 44100, 1, 44100, false, true, OnAudioRead, OnAudioSetPosition);
        //sampleRate = AudioSettings.outputSampleRate;
        //audio.clip = myClip;
    }
    void Start() {
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

    bool start = true;
    int readPerUpdate = 44100 / 10;
    int playForSeconds = 1;
    void Update()
    {
        if (start)
        {
            start = false;
            GetAudioFromServer(44100);
            AudioClip myClip = AudioClip.Create("spotifry", 44100 * playForSeconds, 2, 44100, false, true, GetAudioFromQueue, OnAudioSetPosition);
            audio.clip = myClip;
            audio.PlayDelayed(3);
        }
        else
        {
            GetAudioFromServer(readPerUpdate);
        }
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
                short result = System.BitConverter.ToInt16(inbuffer, 0);
                //if (i == 0) { Debug.Log("result: " + result); }
                queue.Enqueue(result);
                i++;
            }
        }
    }
    void OnAudioSetPosition(int newPosition)
    {
        UnityEngine.Debug.Log("new pos: " + newPosition);
    }
    void GetAudioFromQueue(float[] data)
    {
        //Debug.Log("get audio");
        //Debug.Log("asking for data " + data.Length);
        while(queue.DataLeft() < data.Length)
        {
            GetAudioFromServer(readPerUpdate);
        }
        //Stopwatch sw = new Stopwatch();
        //sw.Start();
        for (int i = 0; i < data.Length; i++)
        {
            float test = queue.Dequeue();
            if (i % 10 == 0) { Debug.Log("test: " + test); }
            data[i] = test;
            /*
            if (i % 500 == 0)
            {
                if (test == 0)
                    test = 1;
                else 
                    test = 0;
            }
                data[i] = test;
            */
            //if (i % 100 == 0)
            //{
            //    UnityEngine.Debug.Log("value: " + data[i]);
            //}
            //Debug.Log("value: " + data[i]);
        }
        //sw.Stop();
        //UnityEngine.Debug.Log("audio get time " + sw.Elapsed.Milliseconds);
    }
}