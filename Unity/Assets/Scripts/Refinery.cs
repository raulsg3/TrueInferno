﻿using UnityEngine;
using System.Collections;

public class Refinery : MonoBehaviour {
    private ResourceManager resourceManager;

	// Use this for initialization
	void Start () {
        resourceManager = GameObject.FindGameObjectWithTag("ResourceManager").GetComponent<ResourceManager>();
	}
	
	// Update is called once per frame
	void Update () {
	}
}
