﻿using UnityEngine;
using System.Collections.Generic;
using System;

public class PathFollower : MonoBehaviour {
    public GameObject tile;
    public float speed = 0.5f;
    public Queue<Tile> path = new Queue<Tile>();
    public Vector3 targetPosition = new Vector3(0.0f, 0.0f, 0.0f);
    public bool moving = false;

    private Building targetBuilding;
    private bool targetReached = false;

    private Vector3 startPosition;
    //private Transform endMarker;
    private float startTime;
    private float journeyLength;

    private TileManager tileManager;
    private WorkerManager workerManager;
    
    // Use this for initialization
    void Start ()
    {
        GameObject goMap = GameObject.FindGameObjectWithTag("Map");
        tileManager = goMap.GetComponent<TileManager>();
        workerManager = GameObject.FindGameObjectWithTag("WorkerManager").GetComponent<WorkerManager>();

        //this.transform.position = new Vector3(0.0f, 1.0f, 0.0f);

        // Encolar camino de prueba
        //testPath();
    }

    // FixedUpdate is called once per 0.33s
    void FixedUpdate ()
    {
        if (path.Count == 0 && this.transform.position == targetPosition && !targetReached)
        {
            //Debug.Log("Worker reached his destination!");

            // Ask soul to execute its task
            SendMessage("executeTask", targetBuilding, SendMessageOptions.RequireReceiver);

            workerManager.disableWorker(this.gameObject);

            targetReached = true;
        }

        if (path.Count > 0 && !moving)
        {
            moving = true;
            Tile tile = path.Dequeue();
            startPosition = this.transform.position;
            targetPosition = new Vector3(tile.posX, this.transform.position.y, tile.posZ);
            this.transform.LookAt(targetPosition);
            //Destroy(tile.transform.gameObject);
            startTime = Time.time;
            journeyLength = Vector3.Distance(startPosition, targetPosition);
        }

        if (moving)
        {
            // Lerp
            float distCovered = (Time.time - startTime) * speed;
            float fracJourney = distCovered / journeyLength;
            this.transform.position = Vector3.Lerp(startPosition, targetPosition, fracJourney);
            // Si llega al destino, meter moving a false
            if (fracJourney >= 1.0f)
            {
                moving = false;
            }
        }
    }

    public void AddToQueue(List<Tile> path)
    {
        foreach (var item in path)
        {
            this.path.Enqueue(item);
        }
    }

    private void testPath()
    {
        GameObject newTileGO = GameObject.Instantiate<GameObject>(tile);
        newTileGO.name = "WayPoint";
        Tile newTile = newTileGO.GetComponent<Tile>();
        var mesh = newTileGO.GetComponent<MeshRenderer>();

        Component.DestroyImmediate(mesh);
        newTile.type = 0;
        newTile.posX = 0;

        uint maxZ = tileManager.getSizeZ();

        for (uint z = 1; z < maxZ; ++z)
        {
            newTile.posZ = z;
            this.path.Enqueue(GameObject.Instantiate<GameObject>(newTileGO).GetComponent<Tile>());
        }

        Destroy(newTileGO);
    }

    public void setBuilding(Building targetBuilding)
    {
        this.targetBuilding = targetBuilding;
    }
}
