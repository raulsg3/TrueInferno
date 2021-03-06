﻿using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class BuildingGenerator : MonoBehaviour {
    public GameObject newBuilding;
    public List<GameObject> newRoad;
    public bool bRoad;
    public bool bShovel;
    public bool bRepairTool;

    public void start() {
        newBuilding = null;
        newRoad = null;
    }

    public void CreateBuilding(GameObject building)
    {
        if (newBuilding)
            Destroy(newBuilding);

        foreach (GameObject go in newRoad){
            Destroy(go);
        }

        newBuilding = Instantiate(building, new Vector3(0f, 0f, 0f), Quaternion.identity) as GameObject;
        Building buildcomp = building.GetComponent<Building>();

        bRepairTool = (buildcomp.validTileType == 4);
        float heightMargin = 0.6f;
        if (bRepairTool)
            heightMargin += 0.7f;

        if (buildcomp)
        {
            newBuilding.transform.position = new Vector3(0f,(float) buildcomp.sizeY / 2 + heightMargin,0f);
            newBuilding.transform.localScale = new Vector3(buildcomp.sizeX, buildcomp.sizeY, buildcomp.sizeZ);
            Renderer rend = newBuilding.GetComponent<Renderer>();
            rend.material.SetColor("_SpecColor", Color.red);
        }
        bRoad = (buildcomp.BuildingType==TileType.ROAD);
        bShovel = (buildcomp.BuildingType == TileType.RUINS);
        bRepairTool = (buildcomp.validTileType == 4);
		UnityEngine.Cursor.visible = true; // TODO Cambiar a false
    }

    public void CreateRoad(List<Tile> path, GameObject road) {
        GameObject roadSegment;
        Building buildcomp = road.GetComponent<Building>();

        foreach (GameObject go in newRoad)
        {
            Destroy(go);
        }

        if (path!=null)
        {

            foreach (Tile tile in path)
            {
                roadSegment = Instantiate(road, new Vector3(tile.posX, buildcomp.sizeY / 2 + 0.1f, tile.posZ), Quaternion.identity) as GameObject;
                roadSegment.transform.localScale = new Vector3(buildcomp.sizeX, buildcomp.sizeY, buildcomp.sizeZ);
                newRoad.Add(roadSegment);
            }
        }
            
    }

}
