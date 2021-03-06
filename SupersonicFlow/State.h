#pragma once
#include <vector>
#include "Mesh.h"
#include <memory>
#include "Boundary.h"
#include <math.h>
#include <algorithm>

class NodeState {
public:

	NodeState() {
		boundary_ = NULL;
	}
	NodeState(std::vector<double> v) {
		size_ = (int)v.size();
		for (int i = 0; i < size_; i++) {
			vals[i] = v[i];
		}
		boundary_ = NULL;
	}
	NodeState(int s) {
		size_ = s;
		for (int i = 0; i < size_; i++) {
			vals[i] = 0.0;
		}
		boundary_ = NULL;
	}

	void operator=(std::vector<double>& v) {
		size_ = (int)v.size();
		for (int i = 0; i < size_; i++) {
			vals[i] = v[i];
		}
		boundary_ = NULL;
	}

	void operator=(NodeState state) {
		size_ = state.size();
		for (int i = 0; i < size_; i++) {
			vals[i] = state.vals[i];
		}
		boundary_ = state.boundary_;
	}

	bool operator==(NodeState& other) {
		if (size_ != other.size()) return false;
		for (int i = 0; i < size_; i++) {
			if (other.vals[i] != vals[i]) return false;
		} 
		return true;
	}

	NodeState operator*(double f) {
		NodeState s = *this;
		for (int i = 0; i < size(); i++) {
			s.vals[i] *= f;
		}
		return s;
	}

	NodeState operator+(const NodeState& state) {
		NodeState s = *this;
		for (int i = 0; i < size(); i++) {
			s.vals[i] += state.vals[i];
		}
		return s;
	}

	NodeState operator-(const NodeState& state) {
		NodeState s = *this;
		for (int i = 0; i < size(); i++) {
			s.vals[i] -= state.vals[i];
		}
		return s;
	}

	NodeState operator/(double f) {
		NodeState s = *this;
		for (int i = 0; i < size(); i++) {
			s.vals[i] /= f;
		}
		return s;
	}

	void operator+=(NodeState state) {
		for (int i = 0; i < size(); i++) {
			vals[i] += state[i];
		}
	}

	void operator-=(NodeState state) {
		for (int i = 0; i < size(); i++) {
			vals[i] -= state[i];
		}
	}

	double& operator[](int i) {
		return vals[i];
	}

	int size() const { return size_; }

	void add(double val) { vals[size_++] = val; }

	void setBoundary(Boundary* boundary) {
		if (boundary_ == NULL) {
			boundary_ = std::shared_ptr<std::vector<Boundary*> >(new std::vector<Boundary*>());
		}
		boundary_->push_back(boundary);
		boundaryType_ = (BoundaryType)std::max((int)boundaryType_, (int)boundary->getType());
	}

	double vals[4];
	bool done = false;
	int size_ = 0;

	std::shared_ptr<std::vector<Boundary*> > boundary_;
	BoundaryType boundaryType_ = BoundaryType::EMPTY;
};

class State
{
public:
	State(Mesh & mesh);
	State();
	~State();

	static State createStateFrom(State& state);

	void initMesh(Mesh& mesh);
	Mesh* getMesh() { return mesh_; }

	std::vector<NodeState>& operator[](int i) { return state_[i]; }

	int getYSize() { return (int) state_.size(); }
	int getXSize() { return (int) state_[0].size(); }

	double getX(int y, int x) { return (*mesh_)[y][x].x; }
	double getY(int y, int x) { return (*mesh_)[y][x].y; }

	NodeState getStateWithBoundary(int y, int x);

	bool isWall(int y, int x) { return mesh_->isWall(y, x); }


	void exportTemperature(std::string file);
	void exportVelocityX(std::string file);
	void exportEnergy(std::string file);
	void exportPressure(std::string file);
	void exportVelocityY(std::string file);

private:
	std::vector<std::vector<NodeState> > state_;
	Mesh* mesh_;
};

NodeState operator*(double d, NodeState state);