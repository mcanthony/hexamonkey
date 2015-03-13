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
class AbstractTypeScopeImplementation : public VariableImplementation
{
protected:
    virtual Variable doGetField(const Variant &key, bool modifiable) override;

    virtual ObjectType* modifiableType() = 0;
    virtual const ObjectType& constType() = 0;
};

/**
 * @brief Type scope implementation given a reference to a \link ObjectType type\endlink
 */
class TypeScopeImplementation : public AbstractTypeScopeImplementation
{
public:
    TypeScopeImplementation(ObjectType& type, bool modifiable);
    TypeScopeImplementation(const ObjectType& type);
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
class ParserTypeScopeImplementation : public AbstractTypeScopeImplementation
{
public:
    ParserTypeScopeImplementation(Parser& parser);
protected:
    virtual ObjectType* modifiableType() override;
    virtual const ObjectType& constType() override;
private:
    Parser& _parser;
};


#endif // TYPESCOPEIMPLEMENTATION_H
