// Copyright 2020 Phyronnaz

#pragma once

#include "CoreMinimal.h"
#include "VoxelFastNoiseBase.h"

template<typename T>
class TVoxelFastNoise_CellularNoise : public T
{
public:
	DEFINE_VOXEL_NOISE_CLASS()

public:
	v_flt GetCellular_2D(v_flt x, v_flt y, v_flt frequency) const;
	v_flt GetCellular_3D(v_flt x, v_flt y, v_flt z, v_flt frequency) const;
	
	void GetVoronoi_2D(v_flt x, v_flt y, v_flt m_jitter, v_flt& out_x, v_flt& out_y) const;
	void GetVoronoiNeighbors_2D(
		v_flt x, v_flt y, 
		v_flt m_jitter, 
		v_flt& out_x0, v_flt& out_y0,
		v_flt& out_x1, v_flt& out_y1, v_flt& out_distance1, 
		v_flt& out_x2, v_flt& out_y2, v_flt& out_distance2, 
		v_flt& out_x3, v_flt& out_y3, v_flt& out_distance3) const;
	
protected:
	template<EVoxelCellularDistanceFunction CellularDistance>
	v_flt SingleCellular_2D(v_flt x, v_flt y) const;
	template<EVoxelCellularDistanceFunction CellularDistance>
	v_flt SingleCellular_3D(v_flt x, v_flt y, v_flt z) const;

	template<EVoxelCellularDistanceFunction CellularDistance>
	v_flt SingleCellular2Edge_2D(v_flt x, v_flt y) const;
	template<EVoxelCellularDistanceFunction CellularDistance>
	v_flt SingleCellular2Edge_3D(v_flt x, v_flt y, v_flt z) const;
	
	template<EVoxelCellularDistanceFunction CellularDistance>
	void SingleVoronoi_2D(v_flt x, v_flt y, v_flt m_jitter, v_flt& out_x, v_flt& out_y) const;

protected:
	template<EVoxelCellularDistanceFunction Enum>
	static v_flt CellularDistance_2D(v_flt vecX, v_flt vecY);
	template<EVoxelCellularDistanceFunction Enum>
	static v_flt CellularDistance_3D(v_flt vecX, v_flt vecY, v_flt vecZ);
};