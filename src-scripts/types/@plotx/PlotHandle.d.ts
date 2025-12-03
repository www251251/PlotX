import { UUID } from "@minecraft";

declare module "@plotx" {
    export type CommentID = number;

    export interface CommentModel {
        id_: CommentID;
        author_: string;
        content_: string;
        time_: number;
    }

    export class PlotHandle implements InstanceClassHelper<PlotHandle> {
        $equals(other: PlotHandle): boolean;

        static get NotForSale(): number;

        markDirty(): void;

        getCoord(): PlotCoord;

        getOwner(): UUID;

        setOwner(owner: UUID): void;

        getName(): string;

        setName(name: string): void;

        isForSale(): boolean;

        getSellingPrice(): number;

        setSellingPrice(price: number): void;

        isMember(member: UUID): boolean;

        getMembers(): string[];

        addMember(member: UUID): void;

        removeMember(member: UUID): void;

        getComments(): CommentModel[];

        getComments(author: UUID): CommentModel[];

        getComment(id: CommentID): optional<CommentModel>;

        addComment(author: UUID, content: string): CommentID;

        removeComment(id: CommentID): void;
    }
}
