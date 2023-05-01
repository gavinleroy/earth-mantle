#include "Pipeline.h"

namespace Pipe {
    // FIXME: I don't think using a vtkAlgorithm will work with
    // the lazy updates. We need to look into this.
    using pipe_output = vtkSmartPointer<vtkAlgorithm>;
    using pipe_input  = vtkSmartPointer<vtkAlgorithm>;

    // What I imagine here is a series of generic pipelines
    // E.G., cell to point data, computing velocity, etc ...
    // XXX: these pipelines may need to store some state
    // to get lazily updated with the rest of the pipeline.
}
