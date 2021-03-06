#ifndef TYPESCOPEIMPLEMENTATION_H
#define TYPESCOPEIMPLEMENTATION_H

#include "core/variable/variable.h"

class Parser;
class ObjectType;

/**
 * @brief Scope implementation that gives access to a \link ObjectType type\endlink's parameters
 *
 * The arguments can either be access by their index or name.
 */
class AbstractTypeScope : public VariableImplementation
{
protected:
    AbstractTypeScope(VariableCollector& collector);

    virtual Variant doGetValue() override;
    virtual Variable doGetField(const Variant &key, bool modifiable, bool createIfNeeded) override;

    virtual ObjectType* modifiableType() = 0;
    virtual const ObjectType& constType() = 0;
};

/**
 * @brief Type scope implementation given a reference to a \link ObjectType type\endlink
 */
class TypeScope : public AbstractTypeScope
{
public:
    TypeScope(VariableCollector& collector, ObjectType& type, bool modifiable);
    TypeScope(VariableCollector& collector, const ObjectType& type);
protected:
    virtual ObjectType* modifiableType() override;
    virtual const ObjectType& constType() override;
private:
    ObjectType* _type;
    const ObjectType& _constType;
};

/**
 * @brief Type scope implementation given a reference to a \link Parser parser\endlink
 */
class ParserTypeScope : public AbstractTypeScope
{
public:
    ParserTypeScope(Parser& parser);
protected:
    virtual ObjectType* modifiableType() override;
    virtual const ObjectType& constType() override;
private:
    Object& _object;
    std::shared_ptr< std::pair<bool, ObjectType> > _sharedType;
};


#endif // TYPESCOPEIMPLEMENTATION_H
