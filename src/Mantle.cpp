#include "Mantle.h"

namespace MantleIO {

    // TODO: one performance bottleneck is reading. We need to make the reader
    // happen on a background worker so we don't block the window interaction
    // while something is getting rendered.
    Mantle::Mantle()
    {
        // XXX: only initialize global reader once.
        if (globalReader != nullptr)
            return;

        globalReader = vtkSmartPointer<vtkNetCDFCFReader>::New();
        globalReader->SetFileName(LocateFile(current_timestep).c_str());
        globalReader->UpdateMetaData();
        globalReader->SetSphericalCoordinates(true);
        globalReader->SetOutputTypeToStructured();

        for (auto &value : MantleAttr::values())
            globalReader->SetVariableArrayStatus(value.c_str(), 1);

#ifndef NDEBUG
        std::cout << std::endl
                  << "--- reading data file [" << current_timestep << "] --- "
                  << std::endl;
        globalReader->GetOutput()->Print(std::cout);
#endif
    }

    std::filesystem::path Mantle::LocateFile(size_t time_step)
    {
        std::stringstream ss = std::stringstream();
        ss << "spherical" << std::setfill('0') << std::setw(3) << current_timestep
           << ".nc";

        std::filesystem::path stem     = std::filesystem::path(data_directory);
        std::filesystem::path resolved = stem.append(ss.str());

#ifndef NDEBUG
        std::cout << "Loading file: " << resolved << std::endl;
        assert(std::filesystem::exists(resolved));
#endif

        return resolved;
    }

    vtkSmartPointer<vtkAlgorithm> Mantle::GetCurrentStream()
    {
        return globalReader;
    }

    void Mantle::Step()
    {
#ifndef NDEBUG
        std::cout << "Stepping Mantle state" << std::endl;
#endif
        current_timestep += 1;

        // Update where the reader is pointing.
        globalReader->SetFileName(LocateFile(current_timestep).c_str());
    }

    // Dataset attributes

    std::vector<MantleAttr> MantleAttr::values()
    {
        auto elems = std::vector<MantleAttr>();
        for (int i = Value::Vx; i <= Value::TempAnom; ++i)
            elems.push_back(MantleAttr(static_cast<Value>(i)));
        return elems;
    }
}
