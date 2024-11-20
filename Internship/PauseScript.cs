using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Pause : MonoBehaviour
{
    public AppManager _am;

    public void PauseAudio()
    {
        
        GameObject lastGameobject = _am.AnimAudioObjectList[_am.AnimAudioObjectList.Count - 1];
        AudioAnimPlayer aap = lastGameobject.GetComponent<AudioAnimPlayer>();

        aap.Pause();

    }

    public void UnPauseAudio()
    {
        GameObject lastGameobject = _am.AnimAudioObjectList[_am.AnimAudioObjectList.Count - 1];
        AudioAnimPlayer aap = lastGameobject.GetComponent<AudioAnimPlayer>();

        aap.Unpause();
    }

}
