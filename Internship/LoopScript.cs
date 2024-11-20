using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.UI;

public class LoopScript : MonoBehaviour
{
    public AppManager _am;

    
    public void LoopAudio()
    {

        Toggle toggle = this.GetComponent<Toggle>();
        

        GameObject lastGameobject = _am.AnimAudioObjectList[_am.AnimAudioObjectList.Count - 1];
        AudioAnimPlayer aap = lastGameobject.GetComponent<AudioAnimPlayer>();

        aap._loopEnabled = toggle.isOn;

       
        aap.LoopAnim();
   
        
        

    }

}
