using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;

public class AppManager : MonoBehaviour
{


    public AudioClip _audioClip;
    public GameObject Model;
    public Avatar Avatar;

    public List<GameObject> AnimAudioObjectList = new List<GameObject>();

    // public GameObject AnimationsTree;

    private GameObject go;

    public void AddAnimationPlayer()
    {

        // GameObject go = AnimAudioObjectList[AnimAudioObjectList.Count - 1];

        GameObject go = Instantiate(Model);
        go.transform.transform.position = new Vector3(0, 0, 0);
        go.transform.transform.rotation = new Quaternion(0,-180,0,0);
        // go.transform.SetParent(AnimationsTree.transform, false);
        go.name = _audioClip.name ;
        AnimAudioObjectList.Add(go);
        AudioAnimPlayer aap = go.AddComponent<AudioAnimPlayer>();

        

        aap._audioClip = _audioClip;

        aap.Avatar = Avatar;
        
      

        
    }





    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
