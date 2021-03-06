#pragma once

#include <unordered_map>
#include <vector>

#include <glbinding/gl/types.h>

#include <Deliberation/Draw/ProgramInterfaceBufferTexture.h>
#include <Deliberation/Draw/ProgramInterfaceFragmentOutput.h>
#include <Deliberation/Draw/ProgramInterfaceSampler.h>
#include <Deliberation/Draw/ProgramInterfaceUniform.h>
#include <Deliberation/Draw/ProgramInterfaceUniformBlock.h>
#include <Deliberation/Draw/ProgramInterfaceVertexAttribute.h>

namespace deliberation
{
class ProgramInterface final
{
public:
    ProgramInterface();
    ProgramInterface(gl::GLuint glProgramName);

    const ProgramInterfaceVertexAttribute *
                                    attribute(const std::string & name) const;
    const ProgramInterfaceUniform * uniform(const std::string & name) const;
    const ProgramInterfaceSampler * sampler(const std::string & name) const;
    const ProgramInterfaceFragmentOutput *
    fragmentOutput(const std::string & name) const;
    const ProgramInterfaceUniformBlock *
    uniformBlock(const std::string & name) const;
    const ProgramInterfaceBufferTexture *
    bufferTexture(const std::string & name) const;

    const ProgramInterfaceVertexAttribute &
                                    attributeRef(const std::string & name) const;
    const ProgramInterfaceUniform & uniformRef(const std::string & name) const;
    const ProgramInterfaceSampler & samplerRef(const std::string & name) const;
    const ProgramInterfaceFragmentOutput &
    fragmentOutputRef(const std::string & name) const;
    const ProgramInterfaceUniformBlock &
    uniformBlockRef(const std::string & name) const;
    const ProgramInterfaceBufferTexture &
    bufferTextureRef(const std::string & name) const;

    const ProgramInterfaceVertexAttribute *
    attributeByLocation(unsigned int location) const;
    const ProgramInterfaceUniform *
    uniformByLocation(unsigned int location) const;
    const ProgramInterfaceSampler *
    samplerByLocation(unsigned int location) const;
    const ProgramInterfaceFragmentOutput *
    fragmentOutputByLocation(unsigned int location) const;

    const std::vector<ProgramInterfaceVertexAttribute> & attributes() const;
    const std::vector<ProgramInterfaceUniform> &         uniforms() const;
    const std::vector<ProgramInterfaceSampler> &         samplers() const;
    const std::vector<ProgramInterfaceFragmentOutput> & fragmentOutputs() const;
    const std::vector<ProgramInterfaceUniformBlock> &   uniformBlocks() const;
    const std::vector<ProgramInterfaceBufferTexture> &  bufferTextures() const;

    std::string toString() const;

private:
    std::vector<ProgramInterfaceVertexAttribute> m_attributes;
    std::unordered_map<std::string, std::size_t> m_attributeIndexByName;
    std::vector<unsigned int>                    m_attributeIndexByLocation;

    std::vector<ProgramInterfaceUniform>         m_uniforms;
    std::unordered_map<std::string, std::size_t> m_uniformIndexByName;
    std::vector<unsigned int>                    m_uniformIndexByLocation;

    std::vector<ProgramInterfaceSampler>         m_samplers;
    std::unordered_map<std::string, std::size_t> m_samplerIndexByName;
    std::vector<unsigned int>                    m_samplerIndexByLocation;

    std::vector<ProgramInterfaceFragmentOutput>  m_fragmentOutputs;
    std::unordered_map<std::string, std::size_t> m_fragmentOutputIndexByName;
    std::vector<unsigned int> m_fragmentOutputIndexByLocation;

    std::vector<ProgramInterfaceUniformBlock>    m_uniformBuffers;
    std::unordered_map<std::string, std::size_t> m_uniformBufferByName;

    std::vector<ProgramInterfaceBufferTexture> m_bufferTextures;
};
}
