namespace deliberation
{

template<typename T>
TypedBlobValueAccessor<T> LayoutedBlob::field(const std::string & name)
{
    return field<T>(m_layout.field(name));
}

template<typename T>
CTypedBlobValueAccessor<T> LayoutedBlob::field(const std::string & name) const
{
    return field<T>(m_layout.field(name));
}

template<typename T>
TypedBlobValueAccessor<T> LayoutedBlob::field(const DataLayoutField & field)
{
    return TypedBlobValueAccessor<T>(m_data, m_layout, field);
}

template<typename T>
CTypedBlobValueAccessor<T> LayoutedBlob::field(const DataLayoutField & field) const
{
    return CTypedBlobValueAccessor<T>(m_data, m_layout, field);
}

template<typename T>
void LayoutedBlob::assign(const std::string & name, const std::vector<T> & values)
{
    field<T>(name).assign(values);
}

}