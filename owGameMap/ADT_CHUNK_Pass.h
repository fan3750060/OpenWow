#pragma once

// A pass that renders the opaque geometry in the scene.
class ADT_CHUNK_Pass : public BasePass
{
public:
	typedef BasePass base;

	ADT_CHUNK_Pass(std::shared_ptr<Scene> scene, std::shared_ptr<PipelineState> pipeline);
	virtual ~ADT_CHUNK_Pass();

	virtual void Visit(IMesh& mesh);

protected:

private:
};